#pragma once

#include <QtCore/QCoreApplication>
#include <QtCore/QAbstractNativeEventFilter>
#include <Windows.h>
#include <process.h>
#include <string>
#include "MariaUI.h"
#include "MariaText.h"
#include "MariaStateManager.h"
#include "MariaInterpreter.h"
#include "MariaTaskManager.h"
#include "MariaFileManager.h"

using namespace std;

class MariaLogic : QApplication {
public:
	MariaLogic(int argc, char *argv[]);
	~MariaLogic(void);

	bool checkValidCommand(string inputText);
	string processCommand(string inputText);
	void terminateProgram();
	void generateTextforUI();

public slots:
	void doShowHide();

private:
	MariaUI*				mariaUI;
	MariaStateManager*		mariaStateManager;
	MariaInterpreter*		mariaInterpreter;
	MariaTaskManager*		mariaTaskManager;
	MariaFileManager*		mariaFileManager;

	void initWindowIcon();
	void initComponents();
	void initTaskManager();
	void initStartingState();
	void initShowHideWrapper();

	void runCommandExit(MariaInputObject* input, MariaStateObject* state);
	string runCommandAddFloatingTask(MariaInputObject* input, MariaStateObject* state);
	string runCommandAddDeadlineTask(MariaInputObject* input, MariaStateObject* state);
	string runCommandAddTimedTask(MariaInputObject* input, MariaStateObject* state);
	string runCommandEditTitle(MariaInputObject* input, MariaStateObject* state);
	string runCommandEditStartTime(MariaInputObject* input, MariaStateObject* state);
	string runCommandEditEndTime(MariaInputObject* input, MariaStateObject* state);
	string runCommandEditDescription(MariaInputObject* input, MariaStateObject* state);
	string runCommandShowDate(MariaInputObject* input, MariaStateObject* state);
	string runCommandShowDateRange(MariaInputObject* input, MariaStateObject* state);
	string runCommandShowAll(MariaInputObject* input, MariaStateObject* state);
	string runCommandSearch(MariaInputObject* input, MariaStateObject* state);
	string runCommandDeleteTask(MariaInputObject* input, MariaStateObject* state);
	string runCommandDeleteAll(MariaInputObject* input, MariaStateObject* state);
	string runCommandMarkDone(MariaInputObject* input, MariaStateObject* state);
	string runCommandMarkUndone(MariaInputObject* input, MariaStateObject* state);
	string runCommandUndo(MariaInputObject* input, MariaStateObject* state);
	string runCommandGoHome(MariaInputObject* input, MariaStateObject* state);
	string runCommandGoCredits(MariaInputObject* input, MariaStateObject* state);
	string runCommandGoHelp(MariaInputObject* input, MariaStateObject* state);
	string runCommandPageUp(MariaInputObject* input, MariaStateObject* state);
	string runCommandPageDown(MariaInputObject* input, MariaStateObject* state);
	string runCommandPageLeft(MariaInputObject* input, MariaStateObject* state);
	string runCommandPageRight(MariaInputObject* input, MariaStateObject* state);

	void saveToFile();
	void addTaskToUI(MariaTask* toAdd, MariaStateObject* state);
	int getPageNumOfTask(MariaTask* task, vector<MariaTask*> listOfTasks, int maxTask);
	//void addStateToQueue(StateType type, );
	
	static void __cdecl doShowHideWrapper(void* mariaLogic);
};
