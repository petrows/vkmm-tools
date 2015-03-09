#pragma once

#include <QJsonValue>
#include <QObject>
#include <QUrlQuery>
#include "VkApiData.h"
#include "Singleton.h"
#include "Log.h"

namespace VKMM {

class VkApiMethod : public QObject
{
	Q_OBJECT
public:
	VkApiMethod(const char * methodId) : id(methodId) {}
	const char * getId() { return id; }
	virtual QUrlQuery getQuery() = 0;
	virtual bool process(QJsonValueRef reply) { return true; }
	void setFinished(bool isOk);

	static VkUser parseUser(const QJsonObject & value);
	static VkAudioPtr parseAudio(const QJsonObject & value);
signals:
	void finished(bool isOk);
protected:
	const char * id;
};

class VkApiGetUser : public VkApiMethod
{
	Q_OBJECT
public:
	VkApiGetUser(quint64 requestUid) : VkApiMethod("users.get"), uid(requestUid) {}
	QUrlQuery getQuery() override;
	bool process(QJsonValueRef reply) override;
	VkUser getUser() { return user; }
protected:
	quint64 uid;
	VkUser user;
};

class VkApiGetAudio : public VkApiMethod
{
	Q_OBJECT
public:
	VkApiGetAudio(qint64 requestUid, qint32 requestCount = 1000, qint32 requestOffset = 0) : VkApiMethod("audio.get"), uid(requestUid), countTotal(0), count(requestCount), offset(requestOffset) {}
	QUrlQuery getQuery() override;
	bool process(QJsonValueRef reply) override;
	VkAudioList & getAudios() { return audios; }
	qint64 getAudiosCount() { return countTotal; }
protected:
	qint64 uid;
	qint32 countTotal;
	qint32 count;
	qint32 offset;
	VkAudioList audios;
};

}

