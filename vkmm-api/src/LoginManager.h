#pragma once

#include "Singleton.h"
#include "Log.h"
#include "ApiCommon.h"
#include <QCommandLineOption>
#include <QDir>
#include <QUrl>

namespace VKMM {

class LoginManager;
class LoginManager : public QObject, public Singleton<LoginManager>
{
	Q_OBJECT
	friend class Singleton<LoginManager>;
protected:
	LoginManager();
	~LoginManager() {}

public:

	enum LoginState {
		LoginUnintilized,
		LoginOk,
		LoginFailed
	};

	void setCmdParams();

	bool init();
	bool login();

	quint64 getUid() { return userId; }
	QString getToken() { return userToken; }

	QUrl getLoginUrl();
	QString getLoginBasePath();
	QString getLoginCookiesPath() { return getLoginBasePath() + "/cookies.dat"; }
	QString getLoginInfoPath() { return getLoginBasePath() + "/info.json"; }

	bool parseLoginUrl(QUrl url);

signals:
	void signalLoginStatus(LoginManager::LoginState state);

private slots:
	void onLoginRequestFinished();

private:
	QCommandLineOption loginTargetDirOption;
	QDir loginTargetDir;

	bool loadUserData();

	time_t	userExpired;
	QString userToken;
	quint64 userId;
};

}
