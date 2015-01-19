#pragma once

#include <QObject>
#include <QUrlQuery>
#include "Singleton.h"
#include "Log.h"

namespace VKMM {

class VkApi;
class VkApiMethod;
class VkApi : public QObject, public Singleton<VkApi>
{
	friend class Singleton<VkApi>;
	Q_OBJECT
protected:
	explicit VkApi(QObject *parent = 0);
	~VkApi() {}
public:
	void request(VkApiMethod * method);
private:
	void sendRequest(VkApiMethod * request);
private slots:
	void onRequestFinished();
	void checkQueryList();
private:
	VkApiMethod *queryCurrent;
	QList<VkApiMethod*> queryList;
};

}

