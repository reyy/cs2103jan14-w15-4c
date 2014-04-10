#include <assert.h> 
#include "MariaUIStateConflict.h"
#include "MariaUI.h"
const float MariaUIStateConflict::TASKBAR_STARTHEIGHT_SCALE = 0.1;
const float MariaUIStateConflict::TASK_STARTHEIGHT_SCALE = 0.2;

MariaUIStateConflict::MariaUIStateConflict(QMainWindow* qmainWindow, vector<MariaTask*> conflictedTask) : MariaUIStateDisplay(qmainWindow, TASK_STARTHEIGHT_SCALE, MAX_ITEM_IN_PAGE) {
	_conflictedTask = conflictedTask;
}

MariaUIStateConflict::~MariaUIStateConflict() {
	clearUITask();
}

void MariaUIStateConflict::initBeginState() {
	clearUITask();

	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(255, 0, 0);
}

void MariaUIStateConflict::initActiveState() {
	for(MariaTask* temp : _conflictedTask) {
		addUITask(temp, MariaUITask::DISPLAY_TYPE::EXPANDED);
	}

	updateUITaskNumber();
	updateUITaskPosition();
	updatePage();
}

void MariaUIStateConflict::initEndState() {
	eraseAllUITask();
}

bool MariaUIStateConflict::timerBeginState() {
	updatePageTitleGUI();
	return false;
}

bool MariaUIStateConflict::timerActiveState() {
	return false;
}

bool MariaUIStateConflict::timerEndState() {
	updatePageTitleGUI();
	return false;
}

void MariaUIStateConflict::updateGUI() {
	updateUITaskNumber();
}