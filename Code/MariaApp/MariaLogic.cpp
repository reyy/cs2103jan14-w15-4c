#include "MariaLogic.h"

MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	mariaUI = new MariaUI();
}

MariaLogic::~MariaLogic(void) {
	delete mariaUI;
}

int main(int argc, char *argv[]) {
	MariaLogic mariaLogic(argc, argv);

	return QApplication::exec();
}