#include "LoginBrowser.h"
#include "ui_LoginBrowser.h"

#include <NetworkAccess.h>
#include <QLayout>
#include <QWebPage>
#include <QWebFrame>
#include <LoginManager.h>
#include <ApiCommon.h>
#include <Log.h>

using namespace VKMM;

LoginBrowser::LoginBrowser(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LoginBrowser)
{
	ui->setupUi(this);

	ui->webView->page()->setNetworkAccessManager(NetworkAccess::instance());
	ui->webView->page()->currentFrame()->setHtml("<center><br/><br/>Press action buttons to begin.</center>");
	connect(ui->webView->page()->currentFrame(), SIGNAL(urlChanged(QUrl)), SLOT(onUrlChanged(QUrl)));
	ui->lblDataDir->setText(LoginManager::instance()->getLoginBasePath());
}

LoginBrowser::~LoginBrowser()
{
	delete ui;
}

void LoginBrowser::on_windowControlButtonBox_rejected()
{
	close();
}

void LoginBrowser::on_btnRelogin_clicked()
{
	ui->webView->setUrl(LoginManager::instance()->getLoginUrl());
}

void LoginBrowser::on_btnLogout_clicked()
{

}

void LoginBrowser::onUrlChanged(QUrl url)
{
	if (url.isEmpty()) return;

	LOG_D(L"New URL: " << url);

	// Test - is ok?
	if (LoginManager::instance()->parseLoginUrl(url))
	{
		ui->webView->page()->currentFrame()->setHtml("<center><br/><br/><div style='color:green;font-weight:bold;'>Login OK</div></center>");
	} else {
		// Error
		LOG_E(L"Login error!");
	}
}
