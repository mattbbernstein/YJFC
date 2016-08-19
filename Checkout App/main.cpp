#include "CheckoutApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	CheckoutApp w;
	w.show();
	return a.exec();
}
