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

CheckoutApp::~CheckoutApp(){
	delete ui;
}

//=====================================================
// SET UP
//=====================================================

void CheckoutApp::setUpSignalsAndSlots() {

	connect(ui->addFencerButton_,    &QPushButton::clicked, this, &CheckoutApp::addFencer);
	connect(ui->saveToFileButton_,   &QPushButton::clicked, this, &CheckoutApp::saveToFile);
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
	connect(ui->fencerList_, &QListWidget::itemDoubleClicked, this, &CheckoutApp::editFencer);

	fencerList_.append(newFencer);

	ui->fencerNameInput_->clear();
	ui->fencerEmailInput_->clear();
	ui->foilCheckBox_->setChecked(false);
	ui->epeeCheckBox_->setChecked(false);
	ui->sabreCheckBox_->setChecked(false);
}
//--------------------------------------------------//

void CheckoutApp::editFencer(QListWidgetItem* item) {

	QString itemText = item->text();
	QString name = itemText.split(" <").first();
	Fencer selected;
	for (int i = 0; i < fencerList_.count(); i++) {
		Fencer testing = fencerList_.at(i);
		if (testing.getName() == name) {
			selected = testing;
			continue;
		}
	}
	if (selected.getName() == QString("NO NAME")){
		return; // Honestly have no idea how you could get here, but its a guard either way
	}

	QList<Equipment> kit = selected.getEquipment();

	EquipmentSelectionDialog dialog(selected.getName(), selected.getGender(), selected.getWeaponsFencing());
	dialog.setKit(kit);
	int resultCode = dialog.exec();
	if (resultCode == EquipmentSelectionDialog::Rejected) {
		return;
	}
	QList<Equipment> newKit = dialog.getKit();
	selected.setAllEquipment(newKit);
	qDebug() << selected.toString();
}
//----------------------------------------------------------------------//

void CheckoutApp::saveToFile() {

	QString path = QFileDialog::getSaveFileName(this, "Save file:", QString(), "*.txt");
	QFile file(path);

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
	
	//connect(client, &Smtp::status, this, &CheckoutApp::emailSent);

	QList<EmailMessage> messages;
	for (int i = 0; i < fencerList_.count(); i++) {
		Fencer fencer = fencerList_.at(i);
		EmailMessage msg;
		msg.from = loginInfo.user;
		msg.to = fencer.getEmail();
		QString subject = QString("Checkout for: ") % ui->tournamentNameInput_->text();
		msg.subject = subject;

		QString body = fencer.toString();
		msg.body = body;

		Smtp* client = new Smtp(loginInfo.user, loginInfo.password, host);
		client->sendMail(msg.from, msg.to, msg.subject, msg.body);
	}
}
//--------------------------------------------------//

void CheckoutApp::emailSent(QString status) {
	qDebug() << status;
}