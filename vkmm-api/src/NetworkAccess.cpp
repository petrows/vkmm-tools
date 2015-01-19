#include "NetworkAccess.h"
#include "NetworkCookieJar.h"

using namespace VKMM;

NetworkAccess::NetworkAccess(QObject *parent) :
	QNetworkAccessManager(parent)
{
	connect(this,SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),SLOT(ignoreSSLErors(QNetworkReply*,QList<QSslError>)));
	cookies.cookiesLoad();
	setCookieJar(&cookies);
}

NetworkAccess::~NetworkAccess()
{
	cookies.cookiesSave();
}

QNetworkReply * NetworkAccess::createRequest (Operation op, const QNetworkRequest & req, QIODevice * outgoingData)
{
	QNetworkRequest req_out = req;
	req_out.setRawHeader(QByteArray("User-Agent"),QByteArray(QString("VKMM/1").toStdString().c_str()));
	QNetworkReply * ret = QNetworkAccessManager::createRequest(op, req_out, outgoingData);
	ret->ignoreSslErrors();
	connect(ret,SIGNAL(error(QNetworkReply::NetworkError)),SLOT(error(QNetworkReply::NetworkError)));
	connect(ret,SIGNAL(downloadProgress(qint64,qint64)),SLOT(progress(qint64, qint64)));
	connect(ret,SIGNAL(sslErrors(QList<QSslError>)),SLOT(ignoreSSLErorsReply(QList<QSslError>)));
	return ret;
}

void NetworkAccess::ignoreSSLErors(QNetworkReply *reply, const QList<QSslError> &errors)
{
	reply->ignoreSslErrors();
}

void NetworkAccess::ignoreSSLErorsReply(const QList<QSslError> &errors)
{
	QNetworkReply * ret = qobject_cast<QNetworkReply*>(sender());
	if (NULL != ret)
	{
		ret->ignoreSslErrors();
	}
}

void NetworkAccess::error(QNetworkReply::NetworkError code)
{
	//qDebug() << "Net error: " << (int)code;
}

void NetworkAccess::progress(qint64 a, qint64 b)
{
	//qDebug() << "Progress: " << a << b;
}
