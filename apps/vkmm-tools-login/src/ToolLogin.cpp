#include "ToolLogin.h"

#include <LoginManager.h>
#include <CommandLine.h>
#include <ApiCommon.h>

using namespace VKMM;

int ToolLogin::init()
{
	CommandLine::instance()->setApplicationDescription("Generate new login data via standard login browser (UI mode)");
	CommandLine::instance()->addHelpOption();
	LoginManager::instance()->setCmdParams();

	CommandLine::instance()->process(*QCoreApplication::instance());

	return 0;
}
