#include "ToolLoginTest.h"

#include <LoginManager.h>
#include <CommandLine.h>
#include <VkApi.h>
#include <VkApiMethods.h>

using namespace VKMM;

int ToolLoginTest::init()
{
	CommandLine::instance()->setApplicationDescription("Test specifed login. If login fails return 1, on ok login return 0.");
	CommandLine::instance()->addHelpOption();
	LoginManager::instance()->setCmdParams();
	
	CommandLine::instance()->process(*QCoreApplication::instance());

	if (!LoginManager::instance()->init()) return 1;

	connect(LoginManager::instance(), SIGNAL(signalLoginStatus(LoginManager::LoginState)), SLOT(onLoginStatus(LoginManager::LoginState)));
	if (!LoginManager::instance()->login()) return 1;

	return 0;
}

void ToolLoginTest::onLoginStatus(LoginManager::LoginState state)
{
	if (LoginManager::LoginOk == state)
	{
		// Login is ok!
		LOG_M(L"Congrats! Login seems to be working... Get our user");

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

		return;
	}

	LOG_E(L"Error! Login state = " << state);
}

void ToolLoginTest::onUserInfo(VkUser user)
{
	LOG_M(L"User: " << user.name << L", id = " << user.id);
	QCoreApplication::instance()->exit(0);
}
