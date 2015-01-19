#pragma once

#include <QDir>
#include <QNetworkCookieJar>
#include <QNetworkCookie>

namespace VKMM {

class NetworkCookieJar : public QNetworkCookieJar
{
	Q_OBJECT
private:

public:
	explicit NetworkCookieJar(QObject *parent = 0);

	void cookiesLoad();
	void cookiesSave();
	void cookiesClean();

	QList<QNetworkCookie> cookiesForUrl ( const QUrl & url );
	
signals:
	
public slots:
	
};

}
