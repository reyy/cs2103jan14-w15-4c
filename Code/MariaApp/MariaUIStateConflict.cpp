#include <assert.h> 
#include "MariaUIStateConflict.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

const float MariaUIStateConflict::TASKBAR_STARTHEIGHT_SCALE = 0.1;
const float MariaUIStateConflict::TASK_STARTHEIGHT_SCALE = 0.2;

MariaUIStateConflict::MariaUIStateConflict(QMainWindow* qmainWindow, MariaTaskManager *taskManager, string conflictTaskTitle) : MariaUIStateDisplay(qmainWindow, taskManager, TASK_STARTHEIGHT_SCALE) {
	_conflictTaskTitle = conflictTaskTitle;
}

MariaUIStateConflict::~MariaUIStateConflict() {
	clearTask();
}

void MariaUIStateConflict::initBeginState() {
	clearTask();

	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(255, 0, 0);
}

void MariaUIStateConflict::initActiveState() {
	vector<MariaTask*> tempList = _taskManager->findTask(_conflictTaskTitle);
	for(MariaTask* temp : tempList) {
		addTask(temp, MariaUITask::DISPLAY_TYPE::EXPANDED);
	}
	updateNumber();
}

void MariaUIStateConflict::initEndState() {
	eraseAllTask();
}

bool MariaUIStateConflict::timerBeginState() {
	return false;
}

bool MariaUIStateConflict::timerActiveState() {
	return false;
}

bool MariaUIStateConflict::timerEndState() {
	return false;
}