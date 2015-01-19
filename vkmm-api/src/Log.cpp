#include "Log.h"

#include <iostream>
#include <ctime>

#include <QDebug>

using namespace VKMM;

Log::Log()
{
	// To start file log - call setLogPath
}

Log::~Log()
{
}

void Log::write(Log::Level level, const char *file, int line, const char *func, const std::wstringstream &str)
{
	// Cut The File
	const char * fnameCut = file;
	for (int x=strlen(fnameCut)-1; x>0; x--)
	{
		if (fnameCut[x] == '/' || fnameCut[x] == '\\')
		{
			fnameCut = fnameCut+x+1;
			break;
		}
	}
	
	std::wstringstream logstr;
	logstr << L"[" << getDate() << L"] " << getLevelShort(level) << L" " << QString::fromLocal8Bit(fnameCut).toStdWString() << L":" << line << L"\t" << str.str();
	std::wcout << logstr.str() << std::endl;
}

std::wstring Log::getDate()
{
	std::time_t t = std::time(NULL);
	wchar_t mbstr[100];
	std::wcsftime(mbstr, sizeof(mbstr), L"%Y-%m-%d %H:%M:%S", std::localtime(&t));
	return std::wstring(mbstr);
}

wchar_t Log::getLevelShort(Level lvl)
{
	switch (lvl)
	{
		case Debug:
			return 'D';
			break;
		case Message:
			return 'M';
			break;
		case Error:
			return 'E';
			break;
	}
	return '?';
}
