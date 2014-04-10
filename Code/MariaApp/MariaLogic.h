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

	bool checkValidCommand(std::string inputText);
	bool processCommand(std::string inputText);
	bool processUndo();
	void terminateProgram();
	void generateTextforUI();

public slots:
	void doShowHide();

private:
	MariaUI				*mariaUI;
	MariaStateManager	*mariaStateManager;
	MariaInterpreter	*mariaInterpreter;
	MariaTaskManager	*mariaTaskManager;
	MariaFileManager	*mariaFileManager;

	void runCommandExit(MariaInputObject* input, MariaStateObject* state);
	void runCommandAddFloatingTask(MariaInputObject* input, MariaStateObject* state);
	void runCommandAddDeadlineTask(MariaInputObject* input, MariaStateObject* state);
	void runCommandAddTimedTask(MariaInputObject* input, MariaStateObject* state);
	void runCommandEditTitle(MariaInputObject* input, MariaStateObject* state);
	void runCommandEditStartTime(MariaInputObject* input, MariaStateObject* state);
	void runCommandEditEndTime(MariaInputObject* input, MariaStateObject* state);
	void runCommandEditDescription(MariaInputObject* input, MariaStateObject* state);
	void runCommandShowDate(MariaInputObject* input, MariaStateObject* state);
	void runCommandShowDateRange(MariaInputObject* input, MariaStateObject* state);
	void runCommandShowAll(MariaInputObject* input, MariaStateObject* state);
	void runCommandSearch(MariaInputObject* input, MariaStateObject* state);
	void runCommandDeleteTask(MariaInputObject* input, MariaStateObject* state);
	void runCommandDeleteAll(MariaInputObject* input, MariaStateObject* state);
	void runCommandMarkDone(MariaInputObject* input, MariaStateObject* state);
	void runCommandMarkUndone(MariaInputObject* input, MariaStateObject* state);
	void runCommandUndo(MariaInputObject* input, MariaStateObject* state);
	void runCommandGoHome(MariaInputObject* input, MariaStateObject* state);
	void runCommandPageUp(MariaInputObject* input, MariaStateObject* state);
	void runCommandPageDown(MariaInputObject* input, MariaStateObject* state);

	static void __cdecl doShowHideWrapper(void* mariaLogic);
};
