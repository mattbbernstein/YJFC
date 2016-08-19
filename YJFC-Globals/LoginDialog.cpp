#include "ui_LoginDialog.h"
#include "LoginDialog.h"

//=============================================
// CONSTRUCTOR/DESTRUCTOR
//=============================================

LoginDialog::LoginDialog(QWidget *parent): QDialog(parent), ui(new Ui::LoginDialog) {
	ui->setupUi(this);
}
//----------------------------------------------//

LoginDialog::LoginDialog(QString defaultUser, QString userLabel, QWidget* parent) : QDialog(parent), ui(new Ui::LoginDialog) {
	ui->setupUi(this);

	ui->usernameLabel_->setText(userLabel);
	ui->userEdit_->setText(defaultUser);

	connect(ui->showPassowrdButton_, &QPushButton::pressed, this, &LoginDialog::showPassword);
	connect(ui->showPassowrdButton_, &QPushButton::released, this, &LoginDialog::hidePassword);
}
//----------------------------------------------//

LoginDialog::~LoginDialog(){}

//==============================================
// MEMBER FUNCTIONS
//==============================================

LoginInformation LoginDialog::getLoginInformation() {
	LoginInformation info;
	info.user = ui->userEdit_->text();
	info.password = ui->passEdit_->text();

	return info;
}
//----------------------------------------------//

//==============================================
// SLOTS
//==============================================

void LoginDialog::showPassword() {
	ui->passEdit_->setEchoMode(QLineEdit::Normal);
}
//---------------------------------------------//

void LoginDialog::hidePassword() {
	ui->passEdit_->setEchoMode(QLineEdit::Password);
}
//---------------------------------------------//