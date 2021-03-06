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
QString formatSafeFilename(QString name);
QString formatSafeFilename(const VkAudioPtr &audio);
void    formatSafeFilenames(VkAudioList & audioList);

QString formatPathSetters(QString path, const VkUser & user);

QString formatSize(quint64 size, QString format = "%.2f {S}");

}

inline std::wostream &operator<<(std::wostream &o, QString s) { o << s.toStdWString(); return o; }
inline std::wostream &operator<<(std::wostream &o, QDir s) { o << s.absolutePath().toStdWString(); return o; }
inline std::wostream &operator<<(std::wostream &o, QUrl s) { o << s.toString().toStdWString(); return o; }
