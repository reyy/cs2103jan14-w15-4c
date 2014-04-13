#include <assert.h>
#include "MariaMacros.h"
#include "MariaUIStateHelp.h"
#include "MariaUI.h"

MariaUIStateHelp::MariaUIStateHelp(QMainWindow* qmainWindow, int screen) : MariaStateObject(qmainWindow) {
	assert(qmainWindow !=NULL);

	_qmainWindow = qmainWindow;

	_helpIndex = screen;
	_helpImage = new QLabel(_qmainWindow);
	_helpImage->setAlignment(Qt::AlignCenter);
	_helpImage->setPixmap(*((MariaUI*)_qmainWindow)->getImageHandler(MariaUI::IMAGE_INDEX_HELP + screen));
	_helpImage->show();
	_doneViewing = false;
	_transitionAuto = true;
}

MariaUIStateHelp::~MariaUIStateHelp() {
	SAFE_DELETE(_helpImage);
}

int MariaUIStateHelp::getHelpIndex() {
	return _helpIndex;
}

void MariaUIStateHelp::setHelpIndex(int index) {
	if(index >= 0 && index < MariaUI::AMOUNT_OF_HELP_IMAGE) {
		_helpIndex = index;
		_helpImage->setPixmap(*((MariaUI*)_qmainWindow)->getImageHandler(MariaUI::IMAGE_INDEX_HELP + index));
	}
}

void MariaUIStateHelp::setDoneViewing() {
	_doneViewing = true;
}

void MariaUIStateHelp::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);
	((MariaUI*)_qmainWindow)->setBackgroundColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);
}

void MariaUIStateHelp::initActiveState() {
	
}

void MariaUIStateHelp::initEndState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);
}

bool MariaUIStateHelp::timerBeginState() {
	_helpImage->setGeometry(QRect(getPosition().x(), getPosition().y(), HELP_IMAGE_WIDTH, HELP_IMAGE_HEIGHT));
	return false;
}

bool MariaUIStateHelp::timerActiveState() {
	if(_doneViewing) {
		return false;
	} else {
		return true;
	}
}

bool MariaUIStateHelp::timerEndState() {
	_helpImage->setGeometry(QRect(getPosition().x(), getPosition().y(), HELP_IMAGE_WIDTH, HELP_IMAGE_HEIGHT));
	return false;
}