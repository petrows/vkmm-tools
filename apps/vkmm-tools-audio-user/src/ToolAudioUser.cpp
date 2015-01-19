#include "ToolAudioUser.h"

#include <LoginManager.h>
#include <CommandLine.h>
#include <VkApi.h>
#include <VkApiMethods.h>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <NetworkAccess.h>

using namespace VKMM;

int ToolAudioUser::init()
{
	CommandLine::instance()->setApplicationDescription("Download audio of selected user");
	CommandLine::instance()->addHelpOption();
	LoginManager::instance()->setCmdParams();
	CommandLine::instance()->addPositionalArgument("<dir>", "Output directory for audio files");
	
	CommandLine::instance()->process(*QCoreApplication::instance());

	// Output direcotry is ok?
	if (CommandLine::instance()->positionalArguments().size() < 1)
	{
		CommandLine::instance()->showHelp(1);
		return 1;
	}
	QString savePath = CommandLine::instance()->positionalArguments().at(0);
	QDir().mkpath(savePath);
	outputDir = QDir(savePath);
	LOG_M(L"Save direcotry: " << outputDir);

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

	LOG_M(L"Request user info");
	VkApiGetUser * userReqest = new VkApiGetUser(LoginManager::instance()->getUid());
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
		LOG_D(L"All done!");

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
	QString outFilePath = outputDir.absoluteFilePath(formatSafeFilename(audios.at(currentItemIndex)));
	LOG_D(L"Downloaded size: " << rep->bytesAvailable() << L", file " << outFilePath);

	QFile outFile(outFilePath);
	outFile.open(QIODevice::WriteOnly);
	outFile.write(rep->readAll());
	outFile.close();

	// Next file?
	if (0 == currentItemIndex)
	{
		// All done!
		LOG_M(L"All done!!!!");
		QCoreApplication::instance()->exit(0);
		return;
	}
	currentItemIndex--;
	startDownloadItem();
}

void ToolAudioUser::startDownloadItem()
{
	// Check - has file on disk?
	if (QFile(outputDir.absoluteFilePath(formatSafeFilename(audios.at(currentItemIndex)))).exists())
	{
		// File exists
		LOG_M(L"File exists - " << outputDir.absoluteFilePath(formatSafeFilename(audios.at(currentItemIndex))));
		currentItemIndex--;
		startDownloadItem();
		return;
	}
	VkAudio item = audios.at(currentItemIndex);
	QNetworkRequest req(item.url);
	QNetworkReply * rep = NetworkAccess::instance()->get(req);
	connect(rep, &QNetworkReply::finished, this, &ToolAudioUser::onFileDownloadFinish);
}
