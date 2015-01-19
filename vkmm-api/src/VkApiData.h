#pragma once

#include <QObject>

namespace VKMM {

struct VkUser {
	QString name;
	QUrl    photo;
	quint64 id;
	VkUser() : id(0) {}
};
typedef QList<VkUser> VkUserList;

struct VkAudio {
	quint64 id;
	qint64 uid;
	QString artist;
	QString title;
	quint32 duration;
	quint64 album;
	QUrl url;
	VkAudio() : id(0),uid(0),duration(0),album(0) {}
};
typedef QList<VkAudio> VkAudioList;

}

