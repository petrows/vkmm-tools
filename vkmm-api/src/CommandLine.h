#pragma once

#include "Singleton.h"

#include <QCommandLineParser>
#include <QDir>

namespace VKMM {

class CommandLine;
class CommandLine : public QCommandLineParser, public Singleton<CommandLine>
{
	friend class Singleton<CommandLine>;
protected:
	CommandLine() {}
	~CommandLine() {}
};

}
