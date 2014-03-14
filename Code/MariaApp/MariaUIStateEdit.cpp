#include "MariaUIStateEdit.h"
#include "MariaUI.h"

MariaUIStateEdit::MariaUIStateEdit(QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
}

MariaUIStateEdit::~MariaUIStateEdit() {
}

void MariaUIStateEdit::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*0.3);
}

void MariaUIStateEdit::initActiveState() {
	
}

void MariaUIStateEdit::initEndState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);
}

bool MariaUIStateEdit::timerBeginState() {
	return false;
}

bool MariaUIStateEdit::timerActiveState() {
	return false;
}

bool MariaUIStateEdit::timerEndState() {
	return false;
}