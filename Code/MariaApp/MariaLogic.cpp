#include "MariaLogic.h"

MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	mariaUI = new MariaUI();
	mariaUI->setStatus(MariaUI::WAIT);
	mariaUI->setState(MariaUI::INTRO);
	mariaUI->setBaseText("add Meeting tomorrow");
	//Somewhere somehow, add this to test the dynamic text thingy.
	/*
	QString test=mariaUI->getUserInput();

	if(test.endsWith("o"))
		mariaUI->setBaseText(QString(test+" world"));
		*/
}

MariaLogic::~MariaLogic(void) {
	delete mariaUI;
}

int main(int argc, char *argv[]) {
	MariaLogic mariaLogic(argc, argv);

	return QApplication::exec();
}