#include "MariaUIStateShow.h"
#include "MariaUI.h"

MariaUIStateShow::MariaUIStateShow(QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
}

MariaUIStateShow::~MariaUIStateShow() {
}

void MariaUIStateShow::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*0.3);
	((MariaUI*)_qmainWindow)->setBackgroundColor(116,30,168);
}

void MariaUIStateShow::initActiveState() {
	
}

void MariaUIStateShow::initEndState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);
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