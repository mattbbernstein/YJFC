#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include "YJFC_globals.h"

class Equipment {

public:
	Equipment();
	Equipment(EquipmentType type, unsigned short id);

	~Equipment();

	// Print the Weapon code and number
	QString toString();

	// Getters and Setters

	unsigned short getId() { return id_; }
	EquipmentType getType() { return type_; }

	// Operators

	bool operator==(const Equipment& rhs) const {
		return  ((type_ == rhs.type_) && (id_ == rhs.id_));
	}

private:
	EquipmentType type_;
	unsigned short id_;

};

#endif // EQUIPMENT_H
