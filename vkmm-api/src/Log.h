#pragma once

#include "ApiCommon.h"
#include "Singleton.h"
#include <sstream>
#include <string>

#define LOG_D(...) {std::wstringstream __logger_stringstream; __logger_stringstream << __VA_ARGS__; Log::instance()->write(Log::Debug, __FILE__ , __LINE__, __FUNCTION__, __logger_stringstream);}
#define LOG_M(...) {std::wstringstream __logger_stringstream; __logger_stringstream << __VA_ARGS__; Log::instance()->write(Log::Message, __FILE__ , __LINE__, __FUNCTION__, __logger_stringstream);}
#define LOG_E(...) {std::wstringstream __logger_stringstream; __logger_stringstream << __VA_ARGS__; Log::instance()->write(Log::Error, __FILE__ , __LINE__, __FUNCTION__, __logger_stringstream);}

namespace VKMM {

class Log;
class Log : public Singleton<Log>
{
	friend class Singleton<Log>;
protected:
	Log();
	~Log();

public:

	enum Level {
		Debug	= 4,
		Message = 1,
		Error	= 0
	};

	void write(Level level, const char * file, int line, const char * func, const std::wstringstream &str);

private:
	std::wstring	getDate();
	wchar_t			getLevelShort(Level lvl);
};

}
