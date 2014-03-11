#pragma once

#include <QtCore/QCoreApplication>
#include <string>
#include "MariaUI.h"
#include "MariaStateManager.h"
#include "MariaInterpreter.h"
#include "MariaTaskManager.h"
#include "MariaFileManager.h"

class MariaLogic : QApplication {
public:
	MariaLogic(int argc, char *argv[]);
	~MariaLogic(void);

	bool processCommand(std::string inputText);
	void terminateProgram();

private:
	MariaUI				*mariaUI;
	MariaStateManager	*mariaStateManager;
	MariaInterpreter	*mariaInterpreter;
	MariaTaskManager	*mariaTaskManager;
	MariaFileManager	*mariaFileManager;

	
};
