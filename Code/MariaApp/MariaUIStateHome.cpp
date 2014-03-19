#include <assert.h> 
#include "MariaUIStateHome.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

const float MariaUIStateHome::TASKBAR_STARTHEIGHT_SCALE=0.25;
const float MariaUIStateHome::TASK_STARTHEIGHT_SCALE=0.35;

MariaUIStateHome::MariaUIStateHome(QMainWindow* qmainWindow,MariaTaskManager *taskManager) : MariaUIStateDisplay(qmainWindow, taskManager, TASK_STARTHEIGHT_SCALE) {
	_clock = new MariaUIClock(_qmainWindow);
}

MariaUIStateHome::~MariaUIStateHome() {
	clearTask();
	delete _clock;
}

void MariaUIStateHome::initBeginState() {
	clearTask();
	_clock->startUpdating();

	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(114,143,22);
}

void MariaUIStateHome::initActiveState() {
	vector<MariaTask*> tempList = _taskManager->findTask("");
	for(MariaTask* temp : tempList){
		addTask(temp, MariaUITask::DISPLAY_TYPE::NORMAL);
	}
}

void MariaUIStateHome::initEndState() {
	eraseAllTask();
}

bool MariaUIStateHome::timerBeginState() {
	_clock->updateGUI(getPosition());
	return false;
}

bool MariaUIStateHome::timerActiveState() {
	return false;
}

bool MariaUIStateHome::timerEndState() {
	_clock->updateGUI(getPosition());
	return false;
}