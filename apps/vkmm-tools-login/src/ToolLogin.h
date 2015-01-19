#pragma once

#include "Singleton.h"

#include <QCommandLineParser>

namespace VKMM {

class ToolLogin;
class ToolLogin : public Singleton<ToolLogin>
{
	friend class Singleton<ToolLogin>;
protected:
	ToolLogin() {}
	~ToolLogin() {}
public:
	int init();
};

}
