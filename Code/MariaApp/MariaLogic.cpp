#include "MariaLogic.h"

MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	mariaUI = new MariaUI();
	mariaIntepreter = new MariaInterpreter();
	mariaTaskManager = new MariaTaskManager();
	mariaFileWriter = new MariaFileWriter();
}

MariaLogic::~MariaLogic(void) {
	delete mariaFileWriter;
	delete mariaTaskManager;
	delete mariaIntepreter;
	delete mariaUI;
}

int main(int argc, char *argv[]) {
	MariaLogic mariaLogic(argc, argv);

	return QApplication::exec();
}