#pragma once

#include "Singleton.h"

#include <LoginManager.h>
#include <QCommandLineParser>
#include <VkApiData.h>

namespace VKMM {

class ToolLoginTest;
class ToolLoginTest : public QObject, public Singleton<ToolLoginTest>
{
	Q_OBJECT
	friend class Singleton<ToolLoginTest>;
protected:
	ToolLoginTest() {}
	~ToolLoginTest() {}
public:
	int init();

private slots:
	void onLoginStatus(LoginManager::LoginState state);
	void onUserInfo(VkUser user);
};

}
