#include "MariaStateObject.h"

const float MariaStateObject::FLOW_FACTOR = 0.05;
const float MariaStateObject::VALUE_THRESHOLD = 1.0;

MariaStateObject::MariaStateObject(QMainWindow* qmainWindow) {
	_qmainWindow = qmainWindow;
	_transitionAuto = false;
}

MariaStateObject::~MariaStateObject() {
}

void MariaStateObject::setStartDestination() {
	_position.setX(_qmainWindow->width());
	_position.setY(0.0);
	_destination.setX(0.0);
	_destination.setY(0.0);
}

void MariaStateObject::setEndDestination() {
	_destination.setX(-_qmainWindow->width());
	_destination.setY(0.0);
}

bool MariaStateObject::updatePosition() {
	
	if (abs(_position.x() - _destination.x())>VALUE_THRESHOLD || 
		abs(_position.y() - _destination.y())>VALUE_THRESHOLD) {
		_position.setX(_position.x() + (_destination.x() - _position.x()) * FLOW_FACTOR);
		_position.setY(_position.y() + (_destination.y() - _position.y()) * FLOW_FACTOR);
		return true;
	} else {
		return false;
	}
}

QPointF MariaStateObject::getPosition() {
	return _position;
}

void MariaStateObject::initBeginState() {
}

void MariaStateObject::initActiveState() {
}

void MariaStateObject::initEndState() {
}

bool MariaStateObject::timerBeginState() {
	return false;
}

bool MariaStateObject::timerActiveState() {
	return false;
}

bool MariaStateObject::timerEndState() {
	return false;
}