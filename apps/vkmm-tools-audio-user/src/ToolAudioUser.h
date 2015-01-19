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
	ToolAudioUser() : currentItemIndex(-1), uid(0), countTotal(0), requestOffset(0), requestCount(1000) {}
	~ToolAudioUser() {}
public:
	int init();

private slots:
	void onLoginStatus(LoginManager::LoginState state);
	void onUserInfo(VkUser userData);
	void onAudioResult(bool isOk);
	void onFileDownloadFinish();

private:
	void startDownloadItem();

	QDir outputDir;

	qint64 currentItemIndex;

	quint64 uid;
	VkUser user;

	quint64 countTotal;
	quint64 requestOffset;
	quint64 requestCount;

	VkAudioList audios;
};

}
