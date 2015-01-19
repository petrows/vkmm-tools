#include "ToolLoginTest.h"

#include <LoginManager.h>
#include <QCoreApplication>

using namespace VKMM;

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	app.setApplicationName("vkmm-tools-logintest");

	int coreInit = ToolLoginTest::instance()->init();
	if (0 != coreInit)
	{
		return coreInit;
	}

	return app.exec();
}
