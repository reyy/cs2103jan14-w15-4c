#include "MariaUIStateSetting.h"
#include "MariaUI.h"

MariaUIStateSetting::MariaUIStateSetting(QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
}

MariaUIStateSetting::~MariaUIStateSetting() {
}

void MariaUIStateSetting::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*0.3);
}

void MariaUIStateSetting::initActiveState() {
	
}

void MariaUIStateSetting::initEndState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);
}

bool MariaUIStateSetting::timerBeginState() {
	return false;
}

bool MariaUIStateSetting::timerActiveState() {
	return false;
}

bool MariaUIStateSetting::timerEndState() {
	return false;
}