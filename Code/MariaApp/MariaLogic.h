#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QAbstractNativeEventFilter>
#include <Windows.h>
#include <process.h>
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
	bool processUndo();
	void terminateProgram();

public slots:
	void doShowHide();
	

private:
	MariaUI				*mariaUI;
	MariaStateManager	*mariaStateManager;
	MariaInterpreter	*mariaInterpreter;
	MariaTaskManager	*mariaTaskManager;
	MariaFileManager	*mariaFileManager;

	
	static void __cdecl doShowHideWrapper(void* mariaLogic);
};
