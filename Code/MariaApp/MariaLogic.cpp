#include "MariaLogic.h"

MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	mariaUI = new MariaUI(this);
	
	mariaUI->setState(MariaUI::INTRO);
	mariaIntepreter = new MariaInterpreter();
	mariaTaskManager = new MariaTaskManager();
	mariaFileWriter = new MariaFileWriter();
	//Put loading intensive stuffs in-between changing state to intro and to other state.
	mariaUI->setState(MariaUI::FOCUS);


	//Below are things that you can edit.
	//mariaUI->setBackgroundColor("#ff88ff");
	mariaUI->setQuestionText("What would you like to do?");
	mariaUI->setStatus(MariaUI::WAIT);
	mariaUI->setBaseText("add Meeting tomorrow for discussion");
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