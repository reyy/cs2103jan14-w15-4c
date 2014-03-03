#include "MariaUIRolling.h"


MariaUIRolling::MariaUIRolling(QMainWindow *parent) {
	_parent=parent;
	_currentState=DEACTIVE;
	_rollingXPos=_parent->width()*0.5+_parent->width()*DEFAULT_BEFORE_X;
	_rollingYPos=_parent->height()*0.5+_parent->height()*DEFAULT_BEFORE_Y;

	_preAnimationTimer = new QTimer(this);
    connect(_preAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStatePreAnimation()));

	_mainAnimationTimer = new QTimer(this);
    connect(_mainAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStateMainAnimation()));

	_posAnimationTimer = new QTimer(this);
    connect(_posAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStatePosAnimation()));
}

MariaUIRolling::~MariaUIRolling() {
	
	delete _preAnimationTimer;
	delete _mainAnimationTimer;
	delete _posAnimationTimer;
}

float MariaUIRolling::getRollingX() {
	return _rollingXPos;
}

float MariaUIRolling::getRollingY() {
	return _rollingYPos;
}

void MariaUIRolling::startMainAnimationTimer() {
	if(!_mainAnimationTimer->isActive()) {
		_mainAnimationTimer->start(1);
	}
}

void MariaUIRolling::stopMainAnimationTimer() {
	_mainAnimationTimer->stop();
}

MariaUIRolling::STATE_TYPE MariaUIRolling::getCurrentState() {
	return _currentState;
}

void MariaUIRolling::updateStatePreAnimation() {
	
	float targetX=_parent->width()*0.5+_parent->width()*DEFAULT_DURING_X;
	float targetY=_parent->height()*0.5+_parent->height()*DEFAULT_DURING_Y;
	
	if(abs(_rollingXPos-targetX)>0.5) {
		_rollingXPos+=(targetX-_rollingXPos)*0.01;
		_rollingYPos+=(targetY-_rollingYPos)*0.01;
	} else {
		_preAnimationTimer->stop();
		_currentState=DURING;
		if(_toEndAnimation) {
			if(!_posAnimationTimer->isActive()) {
				_posAnimationTimer->start(1);
			}
		}
	}
	updateGUI();
}

void MariaUIRolling::updateStateMainAnimation() {
}

void MariaUIRolling::updateStatePosAnimation() {
	
	float targetX=_parent->width()*0.5+_parent->width()*DEFAULT_AFTER_X;
	float targetY=_parent->height()*0.5+_parent->height()*DEFAULT_AFTER_Y;
	
	if(abs(_rollingXPos-targetX)>0.5) {
		_rollingXPos+=(targetX-_rollingXPos)*0.01;
		_rollingYPos+=(targetY-_rollingYPos)*0.01;
	} else {
		_currentState=DONE;
		_posAnimationTimer->stop();
	}

	updateGUI();
}

void MariaUIRolling::updateGUI() {
}

bool MariaUIRolling::startRolling() {
	if(_currentState==DONE||_currentState==DEACTIVE) {
		_currentState=BEFORE;
		
		_rollingXPos=_parent->width()*0.5+_parent->width()*DEFAULT_BEFORE_X;
		_rollingYPos=_parent->height()*0.5+_parent->height()*DEFAULT_BEFORE_Y;

		if(!_preAnimationTimer->isActive()) {
			_preAnimationTimer->start(1);
		}
		_toEndAnimation=false;
		return true;
	} else {
		return false;
	}
}

bool MariaUIRolling::isStartRollingDone() {
	if(_currentState==DURING) {
		return true;
	} else {
		return false;
	}
}

bool MariaUIRolling::endRolling() {
	if(_currentState==BEFORE||_currentState==DURING) {
		_currentState=AFTER;
		_toEndAnimation=true;

		if((!_posAnimationTimer->isActive())&&(!_preAnimationTimer->isActive())) {
			_posAnimationTimer->start(1);
		}
		return true;
	} else {
		return false;
	}
}

bool MariaUIRolling::isEndRollingDone() {
	if(_currentState==DONE) {
		return true;
	} else {
		return false;
	}
}