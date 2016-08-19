#include "Fencer.h"
#include <QStringBuilder>
#include <QTextStream>

//=============================================
// CONSTRUCTOR/DESTRUCTOR
//=============================================
Fencer::Fencer(){
	name_ = QString("NO NAME");
	email_ = QString("NO EMAIL");
	gender_ = Gender::MALE;
	weaponsFencing_ = Weapons(NOT_FENCING);
}
//------------------------------------------//

Fencer::Fencer(QString name, QString email, Gender gender, Weapons fencing) {
	name_ = name;
	email_ = email;
	gender_ = gender;
	weaponsFencing_ = fencing;
}
//------------------------------------------//

Fencer::~Fencer(){}
//------------------------------------------//

//=============================================
// MEMBER FUNCTIONS
//=============================================

void Fencer::addEquipment(Equipment eq) {
	if (equipment_.contains(eq)) {	// Don't add duplicates
		return;
	}

	equipment_.append(eq);
}
//------------------------------------------//

void Fencer::removeEquipment(Equipment eq) {
	equipment_.removeAll(eq);
}
//------------------------------------------//

QString Fencer::printEquipmentList() {
	QString toRet;
	QTextStream stream(&toRet);
	for (int i = 0; i < equipment_.count(); i++) {	// Iterate through the equipment list
		Equipment eq = equipment_[i];
		QString line = equipmentNames[eq.getType()] % ": " % eq.toString();	// NAME: CODE#ID
		if (i != equipment_.count() - 1) {
			line = line % QString("\n");									// New line if not the end of the list
		}
		stream << line;														// Add the line to the stream
	}

	return toRet;
}
//-----------------------------------------//

QString Fencer::toString() {
	QString toRet;
	QTextStream stream(&toRet);

	// FORMAT:	George P. Burdell M EFS
	//			gburdell3@gatech.edu
	//			
	//			EQUIPMENT
	//			==============================

	stream << name_;				
	if (gender_ == MALE) {
		stream << " M ";
	} else {
		stream << " F ";
	}

	if (weaponsFencing_.testFlag(NOT_FENCING)) stream << "NOT FENCING";
	else {
		if (weaponsFencing_.testFlag(FENCING_EPEE)) stream << "E";
		if (weaponsFencing_.testFlag(FENCING_FOIL)) stream << "F";
		if (weaponsFencing_.testFlag(FENCING_SABRE)) stream << "S";
	}
	stream << QString("\n") << email_ << QString("\n\n");
	stream << printEquipmentList();
	stream << QString("\n======================================================\n\n");
	return toRet;
}
//---------------------------------------------------------------------//