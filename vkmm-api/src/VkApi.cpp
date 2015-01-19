#include "LoginManager.h"
#include "NetworkAccess.h"
#include "VkApi.h"

#include <VkApiMethods.h>
#include <Log.h>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>

using namespace VKMM;

VkApi::VkApi(QObject *parent) :
	queryCurrent(NULL)
{

}

void VkApi::request(VkApiMethod *method)
{
	method->setParent(this); // Take own of it...

	if (NULL == queryCurrent)
	{
		// We are idle, we can start for now
		sendRequest(method);
		return;
	}

	// Add method to Query
	queryList.push_back(method);
}

void VkApi::sendRequest(VkApiMethod *request)
{
	queryCurrent = request;

	// Make an URL
	QUrl requestUrl = QString("https://api.vk.com/method/") + request->getId();
	QUrlQuery requestQuery = request->getQuery();
	requestQuery.addQueryItem("access_token", LoginManager::instance()->getToken());
	requestQuery.addQueryItem("version", "5.27");
	requestUrl.setQuery(requestQuery);

	LOG_D(L"Request: " << requestUrl);
	QNetworkRequest req(requestUrl);
	QNetworkReply * rep = NetworkAccess::instance()->get(req);
	connect(rep, SIGNAL(finished()), SLOT(onRequestFinished()));
}

void VkApi::onRequestFinished()
{
	QNetworkReply * ret = qobject_cast<QNetworkReply*>(sender());
	if (NULL != ret && NULL != queryCurrent)
	{
		QByteArray respData = ret->readAll();
		bool parseRes = false;

		//LOG_D(QString::fromUtf8(respData.constData()));

		QJsonParseError error;
		QJsonDocument doc = QJsonDocument::fromJson(respData, &error);
		QJsonObject docRoot = doc.object();
		//LOG_D(L"Json result: " << error.errorString());

		if (!docRoot["response"].isNull())
		{
			parseRes = queryCurrent->process(docRoot["response"]);
		}

		queryCurrent->setFinished(parseRes);
	}

	// Reset query
	queryCurrent->deleteLater();
	queryCurrent = NULL;
	checkQueryList();
}

void VkApi::checkQueryList()
{
	if (NULL != queryCurrent) return; // Some query is running now
	if (queryList.size() > 0)
	{
		VkApiMethod * request = queryList.front();
		queryList.removeFirst();
		sendRequest(request);
	}
}
