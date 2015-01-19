#pragma once

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "NetworkCookieJar.h"
#include "Singleton.h"

namespace VKMM {

class NetworkAccess;
class NetworkAccess : public QNetworkAccessManager, public Singleton<NetworkAccess>
{
	friend class Singleton<NetworkAccess>;
	Q_OBJECT
protected:
	explicit NetworkAccess(QObject *parent = 0);
	~NetworkAccess();

protected:
	QNetworkReply * createRequest (Operation op, const QNetworkRequest & req, QIODevice * outgoingData = 0);

signals:
	
public slots:
    void ignoreSSLErors(QNetworkReply * reply, const QList<QSslError> & errors);
	void ignoreSSLErorsReply(const QList<QSslError> & errors);
	void error(QNetworkReply::NetworkError code);
	void progress(qint64, qint64);

private:
	NetworkCookieJar cookies;
};

}

