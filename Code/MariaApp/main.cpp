
#include <QtCore/QCoreApplication>
#include "MariaUI.h"
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MariaUI m;
	m.show();
	return a.exec();
}
