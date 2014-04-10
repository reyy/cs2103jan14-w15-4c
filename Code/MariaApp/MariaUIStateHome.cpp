#include <assert.h> 
#include "MariaUIStateHome.h"
#include "MariaUI.h"

const float MariaUIStateHome::TASKBAR_STARTHEIGHT_SCALE = 0.24;
const float MariaUIStateHome::TASK_STARTHEIGHT_SCALE = 0.65;

MariaUIStateHome::MariaUIStateHome(QMainWindow* qmainWindow, vector<MariaTask*> weekTask) : MariaUIStateDisplay(qmainWindow, TASK_STARTHEIGHT_SCALE, MAX_ITEM_IN_PAGE) {
	_clock = new MariaUIClock(_qmainWindow);
	_preview = new MariaUIPreview(_qmainWindow);
	_weekTask = weekTask;
}

MariaUIStateHome::~MariaUIStateHome() {
	clearUITask();
	delete _preview;
	delete _clock;
}

void MariaUIStateHome::initBeginState() {
	clearUITask();
	_clock->startUpdating();

	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(114, 143, 22);
}

void MariaUIStateHome::initActiveState() {
	for(MariaTask* temp : _weekTask) {
		addUITask(temp, MariaUITask::DISPLAY_TYPE::NORMAL);
	}

	updateUITaskPosition();
	updatePage();
}

void MariaUIStateHome::initEndState() {
	eraseAllUITask();
}

bool MariaUIStateHome::timerBeginState() {
	_clock->updateGUI(getPosition());
	_preview->updateGUI(getPosition());
	updatePageTitleGUI();
	return false;
}

bool MariaUIStateHome::timerActiveState() {
	return false;
}

bool MariaUIStateHome::timerEndState() {
	_clock->updateGUI(getPosition());
	_preview->updateGUI(getPosition());
	updatePageTitleGUI();
	return false;
}

void MariaUIStateHome::updateGUI() {
	((MariaUI*)_qmainWindow)->forceLogicCall();
}

MariaUIPreview* MariaUIStateHome::getUIPreview() {
	return _preview;
}