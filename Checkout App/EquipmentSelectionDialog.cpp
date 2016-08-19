#include "EquipmentSelectionDialog.h"
#include "ui_EquipmentSelectionDialog.h"
#include <QMessageBox>
#include <QStringList>
#include <QRegExp>
#define DISABLE_ROW(x)	ui->formLayout->itemAt((x), QFormLayout::FieldRole)->widget()->setEnabled(false);
#define ROW2WIDGET(x)	ui->formLayout->itemAt((x), QFormLayout::FieldRole)->widget()

//===========================================
// CONSTRUCTOR/DESTRUCTOR
//===========================================

EquipmentSelectionDialog::EquipmentSelectionDialog(QString name, Gender gender, Weapons fencing, QWidget* parent): QDialog(parent), ui(new Ui::EquipmentSelectionDialog){
	ui->setupUi(this);
	ui->nameLabel_->setText(name);

	// Disable chest protector if theyre guys
	if (gender == Gender::MALE) {
		DISABLE_ROW(EquipmentType::CHEST_PROTECTOR);
	}

	// Disable epee rows if theyre not fencing epee
	if (fencing.testFlag(WeaponFlag::FENCING_EPEE) == 0) {
		DISABLE_ROW(EquipmentType::EPEE);
		DISABLE_ROW(EquipmentType::EPEE_MASK);
	}

	// Disable foil rows if theyre not fencing foil
	if (fencing.testFlag(WeaponFlag::FENCING_FOIL) == 0) {
		DISABLE_ROW(EquipmentType::FOIL);
		DISABLE_ROW(EquipmentType::FOIL_MASK);
		DISABLE_ROW(EquipmentType::FOIL_LAME);
	}

	// Disable sabre rows if theyre not fencing sabre
	if (fencing.testFlag(WeaponFlag::FENCING_SABRE) == 0) {
		DISABLE_ROW(EquipmentType::SABRE);
		DISABLE_ROW(EquipmentType::SABRE_MASK);
		DISABLE_ROW(EquipmentType::SABRE_LAME);
		DISABLE_ROW(EquipmentType::SABRE_GLOVE);
	}
}
//------------------------------------------------------//

EquipmentSelectionDialog::~EquipmentSelectionDialog(){
	delete ui;
}
//------------------------------------------------------//

//==========================================================
// MEMBER FUNCTIONS
//==========================================================

QList<Equipment> EquipmentSelectionDialog::getKit() {

	QList<Equipment> kit;
	if (result() == Rejected) {
		return kit;
	}

	// Iterate through the rows
	for (int row = 0; row < ui->formLayout->rowCount(); row++) {
		QLineEdit* lineEdit = (QLineEdit*)ROW2WIDGET(row);								// Convert to line edit
		QString text = lineEdit->text();												// Full text
		QStringList strList = text.split(QRegExp("\\s|,"), QString::SkipEmptyParts);	// Split into list of individual number
		for (int i = 0; i < strList.count(); i++) {										// Iterate through the lists
			int code = strList.at(i).toInt();											// Grab the code
			if (code > 0) {																// If it's greater than 0
				Equipment eq((EquipmentType)row, (unsigned short)code);					// Make an equipment
				kit.append(eq);															// Put it in the kit
			}
		}
	}

	return kit;
}
//-----------------------------------------------------//

void EquipmentSelectionDialog::setKit(QList<Equipment> kit) {
	for (int i = 0; i < kit.count(); i++) {
		Equipment eq = kit.at(i);
		EquipmentType type = eq.getType();
		int id = eq.getId();
		QLineEdit* edit = (QLineEdit*)(ROW2WIDGET(type));
		QString lineText = edit->text();
		if (lineText == "0") { // Set the line if it's empty
			edit->setText(QString::number(id));
		} else { // Otherwise add a new number to the line
			QString newLine = lineText.append(" ").append(QString::number(id));
			edit->setText(newLine);
		}
	}
}
//-----------------------------------------------------//

bool EquipmentSelectionDialog::testInputs() {

	for (int i = 0; i < ui->formLayout->rowCount(); i++) {
		QLineEdit* lineEdit = (QLineEdit*)(ROW2WIDGET(i));
		QString text = lineEdit->text();
		QStringList idList = text.split(QRegExp("\\s|,"));
		for (int j = 0; j < idList.count(); j++) {
			bool ok;
			int num = idList.at(j).toInt(&ok);
			if (!ok) {
				return false;
			}
		}
	}

	return true;
}
//-----------------------------------------------------//

//===================================================
// SLOTS
//===================================================

void EquipmentSelectionDialog::saveRequested() {
	bool goodInputs = testInputs();

	if (goodInputs){
		accept();
	} else {
		QString text = "Bad Input(s)!\nPlease use only numbers, separate multiple with spaces or commas";
		QMessageBox::critical(0, "Error", text);
	}
}
//---------------------------------------------------//

