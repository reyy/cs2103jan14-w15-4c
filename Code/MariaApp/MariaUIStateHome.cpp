#include "MariaUIStateHome.h"
#include "MariaUI.h"

MariaUIStateHome::MariaUIStateHome(QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
	_clock=new MariaUIClock(_qmainWindow);
}

MariaUIStateHome::~MariaUIStateHome() {
	delete _clock;
}

void MariaUIStateHome::initBeginState() {
	_clock->startUpdating();
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*0.5);
}

void MariaUIStateHome::initActiveState() {
	
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