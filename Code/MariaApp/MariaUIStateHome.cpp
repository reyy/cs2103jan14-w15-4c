#include <assert.h> 
#include "MariaUIStateHome.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

const float MariaUIStateHome::TASKBAR_STARTHEIGHT_SCALE = 0.2;
const float MariaUIStateHome::TASK_STARTHEIGHT_SCALE = 0.65;

MariaUIStateHome::MariaUIStateHome(QMainWindow* qmainWindow, MariaTaskManager *taskManager) : MariaUIStateDisplay(qmainWindow, taskManager, TASK_STARTHEIGHT_SCALE) {
	_clock = new MariaUIClock(_qmainWindow);
	_preview = new MariaUIPreview(_qmainWindow, _taskManager);
}

MariaUIStateHome::~MariaUIStateHome() {
	clearUITask();
	delete _preview;
	delete _clock;
}

void MariaUIStateHome::initBeginState() {
	clearUITask();
	_clock->startUpdating();
	_preview->startUpdating();

	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(114, 143, 22);
}

void MariaUIStateHome::initActiveState() {
	vector<MariaTask*> tempList = _taskManager->findTask("");
	for(MariaTask* temp : tempList) {
		addUITask(temp, MariaUITask::DISPLAY_TYPE::NORMAL);
	}
}

void MariaUIStateHome::initEndState() {
	eraseAllUITask();
}

bool MariaUIStateHome::timerBeginState() {
	_clock->updateGUI(getPosition());
	_preview->updateGUI(getPosition());
	return false;
}

bool MariaUIStateHome::timerActiveState() {
	return false;
}

bool MariaUIStateHome::timerEndState() {
	_clock->updateGUI(getPosition());
	_preview->updateGUI(getPosition());
	return false;
}

void MariaUIStateHome::updateGUI() {
	_preview->updateText();
}