#pragma once

#include <QObject>
#include "Singleton.h"
#include "Log.h"

namespace VKMM {

class VkApi;
class VkApi : public QObject, public Singleton<VkApi>
{
	friend class Singleton<VkApi>;
	Q_OBJECT
protected:
	explicit VkApi(QObject *parent = 0) {}
	~VkApi() {}
};

class VkApiMethod : public QObject
{
	Q_OBJECT
public:
	VkApiMethod(const char * methodId) : id(methodId) {}
	const char * getId() { return id; }
	virtual QString getQuery() = 0;
protected:
	const char * id;
};

class VkApiGetUser : public VkApiMethod
{
public:
	VkApiGetUser(quint64 requestUid) : VkApiMethod("users.get"), uid(requestUid) {}
	QString getQuery();
protected:
	quint64 uid;
};

}

