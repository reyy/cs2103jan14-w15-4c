#pragma once

#include <QtCore/QCoreApplication>
#include <string>
#include "MariaUI.h"
#include "MariaInterpreter.h"
#include "MariaTaskManager.h"
#include "MariaFileManager.h"

class MariaLogic : QApplication {
public:
	MariaLogic(int argc, char *argv[]);
	~MariaLogic(void);

	bool processCommand(std::string inputText);

private:
	MariaUI				*mariaUI;
	MariaInterpreter	*mariaInterpreter;
	MariaTaskManager	*mariaTaskManager;
	MariaFileManager	*mariaFileManager;

};
