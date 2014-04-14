//@author A0111784H
#include <assert.h>
#include "MariaMacros.h"
#include "MariaUICommandBar.h"

const float MariaUICommandBar::DEFAULT_X_POSITION = 0.0;
const float MariaUICommandBar::DEFAULT_Y_POSITION = -200.0;
const float MariaUICommandBar::FLOW_FACTOR = 0.05;
const float MariaUICommandBar::VALUE_THRESHOLD = 5.0;

MariaUICommandBar::MariaUICommandBar(QMainWindow *qmainWindow) {
	assert(qmainWindow != NULL);

	_qmainWindow = qmainWindow;

	_yPosition = DEFAULT_Y_POSITION;
	_yDestination = DEFAULT_Y_POSITION;

	_textbox = new MariaUITextbox(_qmainWindow);
	_status = new MariaUIStatus(_qmainWindow);

	_updateGUITimer = new QTimer(_qmainWindow);
	connect(_updateGUITimer, SIGNAL(timeout()), this, SLOT(updateGUI()));
	updateGUI();
}

MariaUICommandBar::~MariaUICommandBar() {
	SAFE_DELETE(_updateGUITimer);
	SAFE_DELETE(_status);
	SAFE_DELETE(_textbox);
}

float MariaUICommandBar::getPosition() {
	return _yPosition;
}

void MariaUICommandBar::setDestination(float yPosition) {
	_yDestination = yPosition;
	if(!_updateGUITimer->isActive()) {
		_updateGUITimer->start(GUI_UPDATE_FREQUENCY);
	}
}

MariaUITextbox* MariaUICommandBar::getTextbox() {
	return _textbox;
}

MariaUIStatus* MariaUICommandBar::getStatus() {
	return _status;
}

void MariaUICommandBar::updateGUI() {
	if(abs(_yPosition - _yDestination)>VALUE_THRESHOLD) {
		_yPosition += (_yDestination - _yPosition) * FLOW_FACTOR;
	} else {
		_updateGUITimer->stop();
	}

	_textbox->updateGUI(QPointF(DEFAULT_X_POSITION, _yPosition));
	_status->updateGUI(QPointF(DEFAULT_X_POSITION, _yPosition));
}