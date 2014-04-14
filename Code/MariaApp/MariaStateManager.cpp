#include "MariaStateManager.h"

MariaStateManager::MariaStateManager() {
	_currentState = BLANK;
	_currentTransition = BEGIN;
	_currentStateObject = NULL;
	_transitCalled = false;

	_stateBeginTimer = new QTimer(this);
	connect(_stateBeginTimer, SIGNAL(timeout()), this, SLOT(timerBeginState()));

	_stateActiveTimer = new QTimer(this);
	connect(_stateActiveTimer, SIGNAL(timeout()), this, SLOT(timerActiveState()));

	_stateEndTimer = new QTimer(this);
	connect(_stateEndTimer, SIGNAL(timeout()), this, SLOT(timerEndState()));
}

MariaStateManager::~MariaStateManager() {
	delete _stateBeginTimer;
	delete _stateActiveTimer;
	delete _stateEndTimer;
}

void MariaStateManager::initBeginState() {
	if(_stateQueue.size()>0) {
		_currentTransition = BEGIN;
		_currentState = _stateQueue.front();
		_stateQueue.pop();
		_currentStateObject = _stateQueueObject.front();
		_stateQueueObject.pop();
		_currentStateObject->initBeginState();
		_currentStateObject->setStartDestination();

		_transitCalled = false;

		_stateBeginTimer->start(1);
	} else {
		_currentState = BLANK;
	}
}

void MariaStateManager::initActiveState() {
	_currentTransition = ACTIVE;
	_currentStateObject->initActiveState();
	_stateActiveTimer->start(1);
}

void MariaStateManager::initEndState() {
	_currentTransition = END;
	_currentStateObject->initEndState();
	_currentStateObject->setEndDestination();
	_stateEndTimer->start(1);
}

void MariaStateManager::timerBeginState() {
	if(!_currentStateObject->timerBeginState()&&!_currentStateObject->updatePosition()) {
		_stateBeginTimer->stop();
		initActiveState();
		_currentStateObject->timerBeginState();
	}
}

void MariaStateManager::timerActiveState() {
	if(!_currentStateObject->timerActiveState()) {
		_stateActiveTimer->stop();

		if(_transitCalled||_currentStateObject->_transitionAuto) {
			initEndState();
		}
	}
}

void MariaStateManager::timerEndState() {
	if(!_currentStateObject->timerEndState()&&!_currentStateObject->updatePosition()) {
		_stateEndTimer->stop();

		//Defensive strategy:
		//Stop all timers if inherited class do not return
		if(_stateActiveTimer->isActive()) {
			_stateActiveTimer->stop();
		}
		if(_stateActiveTimer->isActive()) {
			_stateActiveTimer->stop();
		}
		if(_stateActiveTimer->isActive()) {
			_stateActiveTimer->stop();
		}
		delete _currentStateObject;

		//Start the next state.
		initBeginState();
	}
}

void MariaStateManager::queueState(StateType type, MariaStateObject* stateObject) {

	assert(type != NULL);
	assert(stateObject != NULL);
	
	_stateQueue.push(type);
	_stateQueueObject.push(stateObject);

	if(getCurrentState() == BLANK) {
		initBeginState();
	}
}

void MariaStateManager::transitState() {
	if(_currentTransition == ACTIVE) {
		if(_stateActiveTimer->isActive()) {
			_transitCalled = true;
		} else {
			initEndState();
		}
	} else {
		_transitCalled = true;
	}
}

StateType MariaStateManager::getCurrentState() {
	return _currentState;
}

MariaStateManager::StateTransition MariaStateManager::getTransition() {
	return _currentTransition;
}

void MariaStateManager::clearQueuedState() {
	while(!_stateQueue.empty()) {
		_stateQueue.pop();
		_stateQueueObject.pop();
	}
}

MariaStateObject* MariaStateManager::getCurrentStateObject() {
	return _currentStateObject;
}

bool MariaStateManager::runActiveUpdate() {
	if(!_stateActiveTimer->isActive()) {
		_stateActiveTimer->start(1);
		return true;
	} else {
		return false;
	}
}