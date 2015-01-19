#ifndef LOGINBROWSER_H
#define LOGINBROWSER_H

#include <QDialog>
#include <QtWebKitWidgets/QWebView>

namespace Ui {
class LoginBrowser;
}

class LoginBrowser : public QDialog
{
	Q_OBJECT

public:
	explicit LoginBrowser(QWidget *parent = 0);
	~LoginBrowser();

private slots:
	void on_windowControlButtonBox_rejected();
	void on_btnRelogin_clicked();
	void on_btnLogout_clicked();

	void onUrlChanged(QUrl url);

private:
	Ui::LoginBrowser *ui;
};

#endif // LOGINBROWSER_H
