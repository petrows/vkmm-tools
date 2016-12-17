#include "ToolAudioUser.h"

#include <LoginManager.h>
#include <CommandLine.h>
#include <VkApi.h>
#include <VkApiMethods.h>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <NetworkAccess.h>

#include <tag.h>
#include <fileref.h>
#include <mpeg/mpegfile.h>
#include <mpeg/id3v2/id3v2tag.h>
#include <mpeg/id3v2/frames/urllinkframe.h>
#include <tstring.h>
#include <math.h>

using namespace VKMM;

int ToolAudioUser::init()
{
	CommandLine::instance()->setApplicationDescription("Download audio of selected user");
	CommandLine::instance()->addHelpOption();
	LoginManager::instance()->setCmdParams();
	CommandLine::instance()->addPositionalArgument("<dir>", "Output directory for audio files");

	QCommandLineOption optionUser("user", "Target user to download (only user ID is supported). Default - current user.");
	optionUser.setDefaultValue(0);
	optionUser.setValueName("id");
	CommandLine::instance()->addOption(optionUser);

	QCommandLineOption optionTags("tags", "(re)write tags to ouput files (id3v2, utf8 forced)");
	CommandLine::instance()->addOption(optionTags);
	
	CommandLine::instance()->process(*QCoreApplication::instance());

	// Output direcotry is ok?
	if (CommandLine::instance()->positionalArguments().size() < 1)
	{
		CommandLine::instance()->showHelp(1);
		return 1;
	}

	targetUid = CommandLine::instance()->value(optionUser).toInt();
	writeTags = CommandLine::instance()->isSet(optionTags);

	if (!LoginManager::instance()->init()) return 1;

	connect(LoginManager::instance(), SIGNAL(signalLoginStatus(LoginManager::LoginState)), SLOT(onLoginStatus(LoginManager::LoginState)));
	if (!LoginManager::instance()->login()) return 1;

	return 0;
}

void ToolAudioUser::onLoginStatus(LoginManager::LoginState state)
{
	if (LoginManager::LoginOk != state)
	{
		LOG_E(L"Error! Login state = " << state);
		QCoreApplication::instance()->exit(state);
		return;
	}

	if (0 == targetUid)
	{
		targetUid = LoginManager::instance()->getUid();
	}

	LOG_M(L"Request user info for id = " << targetUid);
	VkApiGetUser * userReqest = new VkApiGetUser(targetUid);
	connect(userReqest, &VkApiGetUser::finished, [this, userReqest](bool isOk) {
		if (isOk)
		{
			onUserInfo(userReqest->getUser());
		} else {
			LOG_E(L"Request error!");
			QCoreApplication::instance()->exit(1);
		}
	});

	VkApi::instance()->request(userReqest);
}

void ToolAudioUser::onUserInfo(VkUser userData)
{
	user = userData;
	LOG_M(L"Downloading user: " << user.name << L" (id:" << user.id << L")");

	QString savePath = CommandLine::instance()->positionalArguments().at(0);
	QString userPath = formatPathSetters(savePath, user);
	QDir().mkpath(userPath);
	outputDir = QDir(userPath);
	LOG_M(L"Downloading path: " << outputDir);

	VkApiGetAudio * audioReqest = new VkApiGetAudio(user.id, requestCount, 0);
	connect(audioReqest, &VkApiGetAudio::finished, this, &ToolAudioUser::onAudioResult);
	VkApi::instance()->request(audioReqest);
}

void ToolAudioUser::onAudioResult(bool isOk)
{
	VkApiGetAudio * rep = qobject_cast<VkApiGetAudio*>(sender());

	if (!isOk || NULL == rep)
	{
		LOG_E(L"Request error!");
		QCoreApplication::instance()->exit(1);
		return;
	}

	// Got some data...
	if (0 == countTotal)
	{
		// First request!
		countTotal = rep->getAudiosCount();
		LOG_M(L"Total audios: " << countTotal << L", need " << (ceil((double)countTotal/(double)requestCount)-1) << " additional requests");
	}

	if (0 == countTotal)
	{
		// Nothing to do
		LOG_M(L"No audios present for this user");
		QCoreApplication::instance()->exit(0);
		return;
	}

	audios.append(rep->getAudios());

	requestOffset++;
	LOG_M(L"Downloaded list " << audios.size() << L" of " << countTotal);

	quint64 downLoaded = requestOffset * requestCount;
	if (downLoaded >= countTotal)
	{
		// All done!
		LOG_D(L"Get audios list is done");

		// Update audios list size
		// Will test on duplicates and generate file names
		formatSafeFilenames(audios);

		// Start download audios!
		currentItemIndex = audios.size() - 1;
		startDownloadItem();
	} else {
		// Need MORE!
		VkApiGetAudio * audioReqest = new VkApiGetAudio(user.id, requestCount, requestOffset * requestCount);
		connect(audioReqest, &VkApiGetAudio::finished, this, &ToolAudioUser::onAudioResult);
		VkApi::instance()->request(audioReqest);
	}
}

void ToolAudioUser::onFileDownloadFinish()
{
	QNetworkReply * rep = qobject_cast<QNetworkReply*>(sender());
	VkAudioPtr audio = audios.at(currentItemIndex);
	QString outFileName = audio->fileName;
	QString outFilePath = outputDir.absoluteFilePath(outFileName);
	LOG_M(L"Downloaded size: " << formatSize(rep->bytesAvailable()) << L", file: " << outFileName);

	QFile outFile(outFilePath);
	if (outFile.open(QIODevice::WriteOnly))
	{
		outFile.write(rep->readAll());
		outFile.close();

		if (writeTags)
		{
			LOG_D("Rewrite tags in file");
			TagLib::MPEG::File tagFile(outFilePath.toStdString().c_str());
			// Remove all
			tagFile.strip(TagLib::MPEG::File::AllTags);
			tagFile.save();

			TagLib::ID3v2::Tag * tag = tagFile.ID3v2Tag(true);

			TagLib::ID3v2::UserUrlLinkFrame *linkFrame = new TagLib::ID3v2::UserUrlLinkFrame();
			linkFrame->setUrl("https://github.com/petrows/vkmm-tools");
			linkFrame->setTextEncoding(TagLib::String::UTF8);
			tag->addFrame(linkFrame);

			tag->setTitle(TagLib::String(audio->title.toUtf8().constData(),TagLib::String::UTF8));
			tag->setArtist(TagLib::String(audio->artist.toUtf8().constData(),TagLib::String::UTF8));
			tag->setComment(TagLib::String(QString("Saved by vkmm-tools").toUtf8().constData(),TagLib::String::UTF8));
			tagFile.save();
		}
	} else {
		LOG_E(L"Error open file for writing: " << outFilePath);
	}

	currentItemIndex--;
	startDownloadItem();
}

void ToolAudioUser::startDownloadItem()
{
	if (currentItemIndex < 0)
	{
		// All done!
		displaySkipped();
		LOG_M(L"All done!");
		QCoreApplication::instance()->exit(0);
		return;
	}
	
	// Check - has file on disk?
	if (QFile(outputDir.absoluteFilePath(audios.at(currentItemIndex)->fileName)).exists())
	{
		countSkipped++;
		// File exists
		// LOG_D(L"File exists - " << outputDir.absoluteFilePath(formatSafeFilename(audios.at(currentItemIndex))));
		currentItemIndex--;
		startDownloadItem();
		return;
	}
	
	displaySkipped();
	
	VkAudioPtr item = audios.at(currentItemIndex);
	QNetworkRequest req(item->url);
	QNetworkReply * rep = NetworkAccess::instance()->get(req);
	connect(rep, &QNetworkReply::finished, this, &ToolAudioUser::onFileDownloadFinish);
}

void ToolAudioUser::displaySkipped()
{
	if (countSkipped > 0)
	{
		LOG_M(L"Skipped files: " << countSkipped);
		countSkipped = 0;
	}
}
