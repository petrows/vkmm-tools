#include "LoginManager.h"
#include "CommandLine.h"
#include "Log.h"
#include "ApiCommon.h"
#include "NetworkAccess.h"

#include <QUrlQuery>
#include <QDateTime>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>

#include <ctime>

using namespace VKMM;

LoginManager::LoginManager() :
	loginTargetDirOption("login")
{

}

void LoginManager::setCmdParams()
{
	// An option with a value
	loginTargetDirOption.setDefaultValue("./login");
	loginTargetDirOption.setValueName("dir");
	loginTargetDirOption.setDescription("Directory with login cookies and data for target user (generated with vkmm-tools-login)");

	CommandLine::instance()->addOption(loginTargetDirOption);
}

bool LoginManager::init()
{
	LOG_D(CommandLine::instance()->value(loginTargetDirOption));
	loginTargetDir = QDir(CommandLine::instance()->value(loginTargetDirOption));
	LOG_M(L"Using folder " << loginTargetDir << L" for login data");

	if (!loginTargetDir.exists())
	{
		LOG_E(L"Directory " << loginTargetDir << L" not exists");
		loginTargetDir.mkpath(loginTargetDir.absolutePath());
	}

	return true;
}

bool LoginManager::login()
{
	// Try to login...

	// Load user data
	if (!loadUserData())
	{
		emit signalLoginStatus(LoginUnintilized);
		return false;
	}

	// Load login url
	QNetworkRequest request(getLoginUrl());
	QNetworkReply * rep = NetworkAccess::instance()->get(request);
	connect(rep, SIGNAL(finished()), SLOT(onLoginRequestFinished()));

	return true;
}

QUrl LoginManager::getLoginUrl()
{
	QUrl loginUrl("https://oauth.vk.com/authorize");

	QUrlQuery loginQuery;

	loginQuery.addQueryItem("client_id", QString::number(2929902));
	loginQuery.addQueryItem("display","popup");
	loginQuery.addQueryItem("redirect_uri","close.html");
	loginQuery.addQueryItem("response_type","token");

	int scope = 0;
	scope += 2; // Firends
	scope += 8; // Audio
	scope += 1024 ; // Status
	scope += 2048 ; // Wall
	scope += 8192 ; // Wall post
	scope += 262144 ; // Groups

	loginQuery.addQueryItem("scope",QString::number(scope));

	loginUrl.setQuery(loginQuery);

	return loginUrl;
}

QString LoginManager::getLoginBasePath()
{
	return loginTargetDir.absolutePath();
}

bool LoginManager::parseLoginUrl(QUrl url)
{
	if (url.host() == "oauth.vk.com" && url.path() == "/close.html")
	{
		// Auth is OK!
		QUrlQuery hashData;
		hashData.setQuery(url.fragment());
		QString token = hashData.queryItemValue("access_token");
		qint32  exp   = hashData.queryItemValue("expires_in").toInt();
		qint32  uid   = hashData.queryItemValue("user_id").toInt();

		LOG_D(L"Get login for user " << uid << ", expired in " << formatDateTime(std::time(NULL) + exp));

		QJsonObject cfg;
		cfg["token"] = token;
		cfg["exp"] = exp;
		cfg["exp_tms"] = (qint64)(std::time(NULL) + exp);
		cfg["uid"] = uid;

		QJsonDocument doc(cfg);
		QByteArray bytes = doc.toJson();

		QFile cfgFile(getLoginInfoPath());
		if (!cfgFile.open(QIODevice::WriteOnly))
		{
			LOG_E(L"Enable to write config file " << getLoginInfoPath());
			return false;
		}
		cfgFile.write(bytes);
		cfgFile.close();

		// Reload data and return
		return loadUserData();
	}
	return false;
}

void LoginManager::onLoginRequestFinished()
{
	QNetworkReply * rep = qobject_cast<QNetworkReply*>( sender() );
	if (NULL != rep)
	{
		// Redirect?
		QString newLocation = rep->header(QNetworkRequest::LocationHeader).toString();
		rep->deleteLater();

		if (!newLocation.isEmpty())
		{
			LOG_D(L"Follow redirect: " << newLocation);
			QNetworkRequest request(newLocation);
			QNetworkReply * rep = NetworkAccess::instance()->get(request);
			connect(rep, SIGNAL(finished()), SLOT(onLoginRequestFinished()));
			return;
		}

		// Has some data!
		if (parseLoginUrl(rep->url()))
		{
			// All ok! Got new login data...
			LOG_M(L"Login ok, user id = " << userId);
			emit signalLoginStatus(LoginOk);
		} else {
			LOG_E(L"Login failed!");
			emit signalLoginStatus(LoginFailed);
		}
	}
}

bool LoginManager::loadUserData()
{
	QFile cfgFile(getLoginInfoPath());
	if (!cfgFile.open(QIODevice::ReadOnly))
	{
		LOG_E(L"Enable to read config file " << getLoginInfoPath());
		return false;
	}
	QByteArray cfgData = cfgFile.readAll();
	cfgFile.close();
	QJsonDocument document = QJsonDocument::fromJson(cfgData);
	QJsonObject documentRoot = document.object();

	userExpired = documentRoot["exp_tms"].toInt();
	userId = documentRoot["uid"].toInt();
	userToken = documentRoot["token"].toString();

	LOG_D(L"Loaded config for user " << userId);

	return true;
}
