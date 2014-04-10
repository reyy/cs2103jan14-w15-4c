#include "MariaUIStateHelp.h"
#include "MariaUI.h"

MariaUIStateHelp::MariaUIStateHelp(QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow = qmainWindow;
}

MariaUIStateHelp::~MariaUIStateHelp() {
}

void MariaUIStateHelp::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);
}

void MariaUIStateHelp::initActiveState() {
	
}

void MariaUIStateHelp::initEndState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);
}

bool MariaUIStateHelp::timerBeginState() {
	return false;
}

bool MariaUIStateHelp::timerActiveState() {
	return false;
}

bool MariaUIStateHelp::timerEndState() {
	return false;
}