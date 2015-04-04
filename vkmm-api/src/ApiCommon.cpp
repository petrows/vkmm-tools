#include "ApiCommon.h"

#include <QDateTime>

namespace VKMM {

QString formatDateTime(time_t tms)
{
	QDateTime tt;
	tt.setTime_t(tms);
	return tt.toString("yyyy-MM-dd hh:mm:ss");
}

QString formatSafeFilename(QString name)
{
	QString out = name.trimmed();
	out = out.replace("&amp;", "&");
	out = out.replace(QRegExp("\\s+"), " ");

	const char * denySymbols = "\\/?:*|\"'";
	while (0x00 != *denySymbols)
	{
		out = out.replace(*denySymbols, "");
		denySymbols++;
	}

	return out;
}

QString formatSafeFilename(const VkAudioPtr &audio)
{
	QString outName = formatSafeFilename(QString(audio->artist + " - " + audio->title));
	outName = outName.left(128);
	if (outName.isEmpty()) outName = QString("audio-") + QString::number(audio->id);
	return outName;
}

void formatSafeFilenames(VkAudioList &audioList)
{
	QStringList fileNames;
	for (int x = audioList.size() - 1; x>=0; x--)
	{
		VkAudioPtr audio = audioList.at(x);
		// Generate filename
		QString fName = formatSafeFilename(audio);
		QString fNameOut = fName;
		int indexZ = 0;
		while (fileNames.contains(fNameOut.toLower()))
		{
			indexZ++;
			fNameOut = fName + "_" + QString::number(indexZ);
		}
		fileNames.push_back(fNameOut.toLower());
		audio->fileName = fNameOut + ".mp3";
	}
}

QString formatPathSetters(QString path, const VkUser &user)
{
	// Replace user defs
	path = path.replace("%uid%", formatSafeFilename(QString::number(user.id)));
	path = path.replace("%uname%", formatSafeFilename(user.name));
	return path;
}

QString formatSize(quint64 sz, QString format)
{
	QString pt;
	float   out = 0.0f;

	if (sz < 1024L)
	{
		pt  = QObject::tr("bytes");
		out = sz;
	} else if (sz < 1024L*1024L) {
		pt  = QObject::tr("Kb");
		out = (long double)sz/(long double)(1024L);
	} else if (sz < 1024L*1024L*1024L) {
		pt  = QObject::tr("Mb");
		out = (long double)sz/(long double)(1024L*1024L);
	} else {
		pt  = QObject::tr("Gb");
		out = (long double)sz/(long double)(1024L*1024L*1024L);
	}

	QString ret = QString().sprintf(format.toStdString().c_str(),out);
	ret = ret.replace("{S}", pt);
	return  ret;
}

}
