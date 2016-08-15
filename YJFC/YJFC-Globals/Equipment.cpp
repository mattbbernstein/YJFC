#include "Equipment.h"

#include <QStringBuilder>

//==================================================
// CONSTRUCTOR/DESRUCTOR
//==================================================
Equipment::Equipment(){
	type_ = EquipmentType::UNKOWN;
	id_ = 0;
}
//--------------------------------------------------//

Equipment::Equipment(EquipmentType type, unsigned short id) {
	type_ = type;
	id_ = id;
}
//-------------------------------------------------//

Equipment::~Equipment(){}
//-------------------------------------------------//

//=================================================
// MEMBER FUNCTIONS
//=================================================
QString Equipment::toString() {
	QString num;
	num.sprintf("%.3u", id_);
	QString toRet = equipmentCodes[type_] % num;
	return toRet;
}
//------------------------------------------------//