#include "ToolLogin.h"

#include <QtWidgets/QApplication>

#include <Log.h>
#include <LoginManager.h>
#include "ui/LoginBrowser.h"

using namespace VKMM;

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("vkmm-tools-login");

	LOG_D("Login tool test");

	int coreInit = ToolLogin::instance()->init();
	if (0 != coreInit)
	{
		return coreInit;
	}

	if (!LoginManager::instance()->init()) return 1;

	LoginBrowser window;
	window.show();

	return app.exec();
}
