#include <assert.h> 
#include "MariaUIStateConflict.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

const float MariaUIStateConflict::TASKBAR_STARTHEIGHT_SCALE = 0.1;
const float MariaUIStateConflict::TASK_STARTHEIGHT_SCALE = 0.2;

MariaUIStateConflict::MariaUIStateConflict(QMainWindow* qmainWindow, MariaTaskManager *taskManager, string conflictTaskTitle) : MariaUIStateDisplay(qmainWindow, taskManager, TASK_STARTHEIGHT_SCALE, MAX_ITEM_IN_PAGE) {
	_conflictTaskTitle = conflictTaskTitle;
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
	vector<MariaTask*> tempList = _taskManager->findTask(_conflictTaskTitle);
	for(MariaTask* temp : tempList) {
		addUITask(temp, MariaUITask::DISPLAY_TYPE::EXPANDED);
	}
	updateUITaskNumber();
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
}