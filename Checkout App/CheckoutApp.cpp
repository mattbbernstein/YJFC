#include "CheckoutApp.h"
#include "ui_CheckoutApp.h"
#include "EquipmentSelectionDialog.h"
#include "LoginDialog.h"
#include "smtp.h"
#include <QDateTime>
#include <QMessageBox>
#include <QStringBuilder>
#include <QFileDialog>

//#include "SmtpMime"


#include <QDebug>

//====================================================
// CONSTRUCTOR/DESTRUCTOR
//====================================================
CheckoutApp::CheckoutApp(QWidget *parent)	: QMainWindow(parent), ui(new Ui::CheckoutApp){
	ui->setupUi(this);

	// UI Setup
	QDateTime today = QDateTime::currentDateTime();
	ui->tournamentDateEdit_->setDateTime(today);

	// Setup
	setUpSignalsAndSlots();

}

CheckoutApp::~CheckoutApp(){}

//=====================================================
// SET UP
//=====================================================

void CheckoutApp::setUpSignalsAndSlots() {

	connect(ui->addFencerButton_,  &QPushButton::clicked, this, &CheckoutApp::addFencer);
	connect(ui->saveToFileButton_, &QPushButton::clicked, this, &CheckoutApp::saveToFile);
	connect(ui->emailFencersButton_, &QPushButton::clicked, this, &CheckoutApp::emailFencers);
}

//=====================================================
// MEMBER FUNCTIONS
//=====================================================
void CheckoutApp::addFencer() {

	// If they haven't typed a name, warn them
	if (ui->fencerNameInput_->text().isEmpty()) {
		QMessageBox::critical(this, "No Name", "Please enter name for the fencer.");
		return;
	}

	// If they haven't typed an email, warn them
	if (ui->fencerEmailInput_->text().isEmpty()) {
		QMessageBox::critical(this, "No Email", "Please enter an email for this fencer.");
		return;
	}

	if (ui->fencerEmailInput_->text().contains("@") == 0) {
		QMessageBox::critical(this, "Invalid e-mail", "Please give a valid e-mail address.");
		return;
	}

	// Get the weapons flag
	bool validFencer = true;
	Weapons weapons = WeaponFlag::NOT_FENCING;
	if (ui->epeeCheckBox_->isChecked()) weapons |= WeaponFlag::FENCING_EPEE;
	if (ui->foilCheckBox_->isChecked()) weapons |= WeaponFlag::FENCING_FOIL;
	if (ui->sabreCheckBox_->isChecked())weapons |= WeaponFlag::FENCING_SABRE;

	// If the fencer isn't fencing, confirm they still want to be checked out
	if (weapons == WeaponFlag::NOT_FENCING){
		QMessageBox::StandardButtons buttons = QMessageBox::Yes | QMessageBox::No;
		QMessageBox::StandardButton button = QMessageBox::question(this, "Fencer not fencing?", "Are you sure this fencer isn't fencing?", buttons);
		// If this was a mistake cancel
		if (button == QMessageBox::No) {
			return;
		}
	}

	// Get the gender
	Gender gender;
	if (ui->maleGenderButton_->isChecked()) gender = Gender::MALE;
	else gender = Gender::FEMALE;

	// Get the name, email
	QString name = ui->fencerNameInput_->text();
	QString email = ui->fencerEmailInput_->text();
	QList<Equipment> kit;

	// As long as a weapons is selected, set up a checkout dialog
	if (weapons != WeaponFlag::NOT_FENCING) {
		EquipmentSelectionDialog equipmentDialog(name, gender, weapons);
		int resultCode = equipmentDialog.exec();
		if (resultCode == QDialog::Rejected) { // If it was rejected, return
			return;
		}
		kit = equipmentDialog.getKit();	// Otherwise get the kit
	}

	// Create the fencer
	Fencer newFencer(name, email, gender, weapons);
	if (kit.count() > 0) {							// If it has a kit
		for (int i = 0; i < kit.count(); i++) {		// For each equipment item
			newFencer.addEquipment(kit.at(i));			// Add it to the fencers equipment
		}
	}

	QString listLabel = name % " <" % email %">";
	ui->fencerList_->addItem(listLabel);

	fencerList_.append(newFencer);

	//qDebug() << newFencer.toString();
	//qDebug() << fencerList_.count();
}
//--------------------------------------------------//
void CheckoutApp::saveToFile() {

	QString path = QFileDialog::getSaveFileName(this, "Save file:", QString(), "*.txt");
	QFile file(path);
	//if (!file.exists()) {
	//	QDir().mkpath(path);
	//}

	if (file.open(QIODevice::WriteOnly)) {
		QTextStream writer(&file);
		writer << QString("YJFC checkout for: ") << ui->tournamentNameInput_ << QString("\n");
		writer << ui->tournamentDateEdit_->date().toString("dd/MM/YYYY");
		for (int i = 0; i < fencerList_.count(); i++) {
			Fencer currFencer = fencerList_.at(i);
			QString fencerData = currFencer.toString();
			writer << fencerData << QString("\n");
		}
	}

	file.close();
}
//--------------------------------------------------//
void CheckoutApp::emailFencers() {

	LoginDialog loginDialog("", "E-mail address");
	int resultCode = loginDialog.exec();
	if (resultCode == LoginDialog::Rejected) {
		return;
	}

	LoginInformation loginInfo = loginDialog.getLoginInformation();
	QString host("smtp.gmail.com");
	Smtp* client = new Smtp(loginInfo.user, loginInfo.password, host);

	QList<EmailMessage> messages;
	for (int i = 0; i < fencerList_.count(); i++) {
		Fencer fencer = fencerList_.at(i);
		EmailMessage msg;
		msg.from = loginInfo.user;
		msg.to = fencer.getEmail();
		QString subject = QString("Checkout for: ") % ui->tournamentNameInput_->text();
		msg.subject = subject;

		QString opening("If you are receiving this you have checked out items for: ");
		opening = opening % ui->tournamentNameInput_->text() % QString("\n");
		opening = opening % "Below is your checkout information\n\n";

		QString body = opening % fencer.toString();
		msg.body = body;

		messages.append(msg);
	}

	for (int i = 0; i < messages.count(); i++) {
		EmailMessage msg = messages.at(i);
		client->sendMail(msg.from, msg.to, msg.subject, msg.body);
	}

	//SmtpClient client("smtp.gmail.com", 465, SmtpClient::SslConnection);
	//client.setUser(loginInfo.user);
	//client.setPassword(loginInfo.password);

	//client.connectToHost();
	//client.login();
	////QList<MimeMessage> messageList;
	//for (int i = 0; i < fencerList_.count(); i++) {
	//	Fencer fencer = fencerList_.at(i);
	//	MimeMessage message;

	//	message.setSender(new EmailAddress(loginInfo.user, QString("Yellow Jacket Fencing")));
	//	message.addRecipient(new EmailAddress(fencer.getEmail(), fencer.getName()));
	//	QString subject = QString("Checkout for: ") % ui->tournamentNameInput_->text();
	//	message.setSubject(subject);

	//	QString opening("If you are receiving this you have checked out items for: ");
	//	opening = opening % ui->tournamentNameInput_->text() % QString("\n");
	//	opening = opening % "Below is your checkout information\n\n";
	//	MimeText mimeOpening(opening);

	//	MimeText text(fencer.toString());

	//	message.addPart(&mimeOpening);
	//	message.addPart(&text);

	//	client.sendMail(message);
	//	//messageList.append(message);
	//}

	////for (int i = 0; i < messageList.count(); i++) {
	////	MimeMessage msg = messageList[i];
	////	client.sendMail(msg);
	////}

	//client.quit();
}
//--------------------------------------------------//