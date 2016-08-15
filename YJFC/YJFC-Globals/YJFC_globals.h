#ifndef YJFC_GLOBALS_H
#define YJFC_GLOBALS_H

#include <QMap>
#include <QString>
#include <QFlags>

enum EquipmentType {
	UNKOWN,
	JACKET,
	GLOVE,
	PLASTRON,
	KNICKERS,
	BAG,
	// EPEE SPECIFIC
	EPEE,
	EPEE_MASK,
	// FOIL SPECIFIC
	FOIL,
	FOIL_MASK,
	FOIL_LAME,
	// SABRE SPECIFIC
	SABRE,
	SABRE_MASK,
	SABRE_LAME,
	SABRE_GLOVE
};

enum WeaponFlag {
	NOT_FENCING  = 0x0,
	FENCING_FOIL = 0x1,
	FENCING_EPEE = 0x2,
	FENCING_SABRE = 0x4
};
Q_DECLARE_FLAGS(Weapons, WeaponFlag);
Q_DECLARE_OPERATORS_FOR_FLAGS(Weapons);

enum Gender {
	MALE,
	FEMALE
};

static QMap<EquipmentType, QString> equipmentCodes = { {JACKET, "JK"}, {GLOVE , "GV"},
												    {PLASTRON, "PL"}, {KNICKERS, "KN"},  {BAG, "BG"},
												    {EPEE, "EP"}, {EPEE_MASK, "EM"},
												    {FOIL, "FO"}, {FOIL_MASK, "FM"}, {FOIL_LAME, "FL"},
												    {SABRE, "SB"}, {SABRE_MASK, "SM"}, {SABRE_LAME, "SL"}, {SABRE_GLOVE, "SG"},
													{UNKOWN, "XX"}
};

static QMap<EquipmentType, QString> equipmentNames = { { JACKET, "Jacket" },{ GLOVE , "Glove" },
													   { PLASTRON, "Plastron" },{ KNICKERS, "Knickers" },{ BAG, "Bag" },
													   { EPEE, "Epee" },{ EPEE_MASK, "Epee Mask" },
													   { FOIL, "Foil" },{ FOIL_MASK, "Foil Mask" },{ FOIL_LAME, "Foil Lame" },
													   { SABRE, "Sabre" },{ SABRE_MASK, "Sabre Mask" },{ SABRE_LAME, "Sabre Lame" },{ SABRE_GLOVE, "Sabre Glove" },
													   { UNKOWN, "XX"}
};

#endif