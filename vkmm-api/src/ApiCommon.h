#pragma once

#include <QTextStream>
#include <string>
#include <sstream>
#include <QDir>
#include <QUrl>

#include "VkApiData.h"

namespace VKMM {

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define WFILE WIDE1(__FILE__)

QString formatDateTime(time_t tms);
inline QString formatSafeFilename(QString name) { return name; }
inline QString formatSafeFilename(VkAudio audio) { return formatSafeFilename(audio.artist + " - " + audio.title + ".mp3"); }

}

inline std::wostream &operator<<(std::wostream &o, QString s) { o << s.toStdWString(); return o; }
inline std::wostream &operator<<(std::wostream &o, QDir s) { o << s.absolutePath().toStdWString(); return o; }
inline std::wostream &operator<<(std::wostream &o, QUrl s) { o << s.toString().toStdWString(); return o; }
