#include "ToolLoginTest.h"

#include <LoginManager.h>
#include <CommandLine.h>
#include <VkApi.h>

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

		// QCoreApplication::exit(0);

		VkApiGetUser * userReqest = new VkApiGetUser(LoginManager::instance()->getUid());
		return;
	}

	LOG_E(L"Error! Login state = " << state);
}
