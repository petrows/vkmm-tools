#pragma once

#include "Singleton.h"

#include <LoginManager.h>
#include <QCommandLineParser>
#include <VkApiData.h>

namespace VKMM {

class ToolAudioUser;
class ToolAudioUser : public QObject, public Singleton<ToolAudioUser>
{
	Q_OBJECT
	friend class Singleton<ToolAudioUser>;
protected:
	ToolAudioUser() : writeTags(false), currentItemIndex(-1), targetUid(0), countSkipped(0), countTotal(0), requestOffset(0), requestCount(1000) {}
	~ToolAudioUser() {}
public:
	int init();

private slots:
	void onLoginStatus(LoginManager::LoginState state);
	void onUserInfo(VkUser userData);
	void onAudioResult(bool isOk);
	void onFileDownloadFinish();
	void displaySkipped();

private:
	void startDownloadItem();

	QDir outputDir;
	bool writeTags;

	qint64 currentItemIndex;

	qint64 targetUid;
	QString targetDomain;
	VkUser user;

	quint64 countSkipped;
	quint64 countTotal;
	quint64 requestOffset;
	quint64 requestCount;

	VkAudioList audios;
};

}
