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

	const char * denySymbols = "\\/?:*|\"";
	while (0x00 != *denySymbols)
	{
		out = out.replace(*denySymbols, "");
		denySymbols++;
	}

	return out;
}

QString formatSafeFilename(VkAudio audio)
{
	QString outName = formatSafeFilename(QString(audio.artist + " - " + audio.title));
	outName = outName.left(128);
	if (outName.isEmpty()) outName = QString("audio-") + QString::number(audio.id);
	return outName + ".mp3";
}

}
