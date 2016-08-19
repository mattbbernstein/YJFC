#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

struct LoginInformation {
	QString user;
	QString password;
};

namespace Ui {
	class LoginDialog;
}

class LoginDialog : public QDialog
{
	Q_OBJECT

public:
	LoginDialog(QWidget *parent = 0);
	LoginDialog(QString defaultUser = "", QString userLabel = "Username", QWidget* parent=0);
	~LoginDialog();

	LoginInformation getLoginInformation();

private:
	Ui::LoginDialog* ui;

private slots:
	void showPassword(); // Displays the password on button press
	void hidePassword(); // Hides the password on button release

};

#endif // LOGINDIALOG_H
