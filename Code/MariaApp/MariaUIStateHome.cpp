#include "MariaUIStateHome.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

const float MariaUIStateHome::TASKBAR_STARTHEIGHT_SCALE=0.35;
const float MariaUIStateHome::TASK_STARTHEIGHT_SCALE=0.5;
const float MariaUIStateHome::TASK_STARTHEIGHT_DIFFERENCE=16.0;

MariaUIStateHome::MariaUIStateHome(MariaTaskManager *taskManager,QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
	_taskManager=taskManager;

	_clock=new MariaUIClock(_qmainWindow);
}

MariaUIStateHome::~MariaUIStateHome() {
	delete _clock;
}

void MariaUIStateHome::initBeginState() {
	_clock->startUpdating();
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(255,127,39);
}

void MariaUIStateHome::initActiveState() {
	vector<MariaTask*> tempList = _taskManager->findTask("");
	for(MariaTask* temp : tempList){
		addTask(*temp);
	}
}

void MariaUIStateHome::initEndState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);
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

void MariaUIStateHome::addTask(MariaTask task) {
	MariaUITask *temp = new MariaUITask(_qmainWindow,task,_qmainWindow->width()-TEXTBOX_X_OFFSET*2);

	temp->setPosition(QPointF(_qmainWindow->width(),_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*_taskStack.size()));
	temp->setDestination(QPointF(TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*_taskStack.size()));
	temp->show();

	_taskStack.push_back(temp);
}

void MariaUIStateHome::eraseTask(int index) {

}

void MariaUIStateHome::clearTask() {
	for(int i=0;i<_taskStack.size();i++) {
		delete _taskStack.at(i);
	}

	while(_taskStack.size()>0) {
		_taskStack.pop_back();
	}
}