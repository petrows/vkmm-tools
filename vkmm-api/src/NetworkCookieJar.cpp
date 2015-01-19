#include "NetworkCookieJar.h"
#include "Log.h"
#include "LoginManager.h"

#include <QFile>
#include <QDateTime>
#include <QStringList>
#include <QNetworkCookie>

using namespace VKMM;

NetworkCookieJar::NetworkCookieJar(QObject *parent) :
	QNetworkCookieJar(parent)
{}

void NetworkCookieJar::cookiesLoad()
{
	LOG_D("Loading cookies from " << LoginManager::instance()->getLoginCookiesPath());

	QFile f(LoginManager::instance()->getLoginCookiesPath());
	if (!f.open(QIODevice::ReadOnly))
		return; // Error!

	QString c_data = f.readAll().constData();
	QStringList c_data_arr = c_data.split("\n");

	QList<QNetworkCookie> clist;

	for (int x=0; x<c_data_arr.size(); x++)
	{
		QStringList cdd = c_data_arr.at(x).split("\t");
		if (cdd.size() != 5) continue;

		QNetworkCookie c_out;
		c_out.setDomain(cdd.at(0));
		c_out.setName(QByteArray(cdd.at(1).toStdString().c_str()));
		c_out.setValue(QByteArray(cdd.at(2).toStdString().c_str()));
		c_out.setPath(cdd.at(3));
		c_out.setExpirationDate(QDateTime::fromTime_t(cdd.at(4).toInt()));

		clist.append(c_out);
	}
	f.close();

	this->setAllCookies(clist);
	LOG_D("Loaded cookies " << clist.size());
}

void NetworkCookieJar::cookiesSave()
{
	LOG_D("Saving cookies to " << LoginManager::instance()->getLoginCookiesPath());

	QList<QNetworkCookie> clist = allCookies();

	QFile f(LoginManager::instance()->getLoginCookiesPath());
	if (!f.open(QIODevice::WriteOnly|QIODevice::Truncate))
		return; // Error!

	QStringList list_out;

	for (int x=0; x<clist.size(); x++)
	{
		QNetworkCookie c = clist.at(x);
		if (c.isSessionCookie()) continue;

		QStringList cookie_text;
		cookie_text << c.domain() << c.name() << c.value() << c.path() << QString::number(c.expirationDate().toTime_t());
		list_out << cookie_text.join("\t");
	}
	f.write(list_out.join("\n").toStdString().c_str());
	f.close();
}

void NetworkCookieJar::cookiesClean()
{
	LOG_D("Cleaning cookies in " << LoginManager::instance()->getLoginCookiesPath());

	QFile f(LoginManager::instance()->getLoginCookiesPath());
	if (f.open(QIODevice::WriteOnly|QIODevice::Truncate))
	{
		f.write("");
		f.close();
	}
	QList<QNetworkCookie> clist;
	setAllCookies(clist);
}

QList<QNetworkCookie> NetworkCookieJar::cookiesForUrl(const QUrl &url)
{
	if (!url.isValid()) return QList<QNetworkCookie>();
	return QNetworkCookieJar::cookiesForUrl(url);
}
