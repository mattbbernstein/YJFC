#ifndef CHECKOUTAPP_H
#define CHECKOUTAPP_H

#include <QtWidgets/QMainWindow>
#include <QListWidgetItem>
#include "Fencer.h"

struct EmailMessage {
	QString from;
	QString to;
	QString subject;
	QString body;
};

namespace Ui {
	class CheckoutApp;
}

class CheckoutApp : public QMainWindow{
	Q_OBJECT

public:
	CheckoutApp(QWidget *parent = 0);
	~CheckoutApp();

private:
	Ui::CheckoutApp* ui;
	QList<Fencer> fencerList_;
	int sentMessages_;
	
	void setUpSignalsAndSlots();

private slots:
	void addFencer();
	void editFencer(QListWidgetItem* item);
	void saveToFile();
	void emailFencers();
	void emailSent(QString status);
};

#endif // CHECKOUTAPP_H
