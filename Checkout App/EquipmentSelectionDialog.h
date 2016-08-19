#ifndef EQUIPMENTSELECTIONDIALOG_H
#define EQUIPMENTSELECTIONDIALOG_H

#include <QDialog>
#include "Equipment.h"

//struct Kit {
//	unsigned short jk;
//	unsigned short pl;
//	unsigned short cp;
//	unsigned short kn;
//	unsigned short bg;
//	unsigned short gv;
//	unsigned short* ep;
//	unsigned short em;
//	unsigned short* fo;
//	unsigned short fm;
//	unsigned short fl;
//	unsigned short* sb;
//	unsigned short sm;
//	unsigned short sl;
//	unsigned short sg;
//	int numEpees;
//	int numFoils;
//	int numSabres;
//};

namespace Ui {
	class EquipmentSelectionDialog;
}

class EquipmentSelectionDialog : public QDialog
{
	Q_OBJECT

public:
	EquipmentSelectionDialog(QString name, Gender gender, Weapons fencing, QWidget* parent = 0);
	~EquipmentSelectionDialog();

	QList<Equipment> getKit();

private:
	Ui::EquipmentSelectionDialog* ui;

	bool testInputs();

private slots:
	void saveRequested();
};

#endif // EQUIPMENTSELECTIONDIALOG_H
