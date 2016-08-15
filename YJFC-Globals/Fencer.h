#ifndef FENCER_H
#define FENCER_H

#include "YJFC_globals.h"
#include "Equipment.h"

#include <QList>

class Fencer{

public:
	Fencer();
	Fencer(QString name, QString email, Gender gender, Weapons fencing);
	~Fencer();

	// Print the equipment list
	QString printEquipmentList();

	// Print full fencer information
	QString toString();

	// Adds a piece of equipment to the fencers list
	void addEquipment(Equipment eq);

	// Removes a piece of equipment from the fencers list
	void removeEquipment(Equipment eq);

	// Getters and Setters

	QString getName() { return name_; }
	QString getEmail() { return email_; }
	void setEmail(QString addr) { email_ = addr; }


private:

	QString name_;
	QString email_;
	Gender gender_;
	Weapons weaponsFencing_;
	QList<Equipment> equipment_;
};

#endif