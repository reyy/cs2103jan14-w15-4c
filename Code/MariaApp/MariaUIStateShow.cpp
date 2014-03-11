#include "MariaUIStateShow.h"
#include "MariaUI.h"

const float MariaUIStateShow::TASK_STARTHEIGHT_SCALE=0.1;

MariaUIStateShow::MariaUIStateShow(QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
}

MariaUIStateShow::~MariaUIStateShow() {
}

void MariaUIStateShow::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASK_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(116,30,168);
}

void MariaUIStateShow::initActiveState() {
	
}

void MariaUIStateShow::initEndState() {
}

bool MariaUIStateShow::timerBeginState() {
	return false;
}

bool MariaUIStateShow::timerActiveState() {
	return false;
}

bool MariaUIStateShow::timerEndState() {
	return false;
}