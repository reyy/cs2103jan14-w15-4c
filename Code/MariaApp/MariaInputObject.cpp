#include "MariaMacros.h"
#include "MariaInputObject.h"

MariaInputObject::MariaInputObject() {
	_commandType = INVALID;

	_startTime = NULL;
	_endTime = NULL;
	_editTime = NULL;
}

MariaInputObject::~MariaInputObject() {
	if (_startTime != NULL) {
		SAFE_DELETE(_startTime);
	}

	if (_endTime != NULL) {
		SAFE_DELETE(_endTime);
	}

	if (_editTime != NULL) {
		SAFE_DELETE(_editTime);
	}
}

void MariaInputObject::setOriginalInput(string originalInput) {
	_originalInput = originalInput;
}

void MariaInputObject::setCommandType(COMMAND_TYPE newCommandType) {
	_commandType = newCommandType;
}

void MariaInputObject::setTitle(string newTitle) {
	_title = newTitle;
}

void MariaInputObject::setEditField(string newEditField) {
	_editField = newEditField;
}

void MariaInputObject::setOptionID(int newOptionID) {
	_optionID = newOptionID;
}

void MariaInputObject::setStartTime(MariaTime* newStartTime) {
	if (_startTime != NULL) {
		delete _startTime;
	}

	_startTime = newStartTime;
}

void MariaInputObject::setEndTime(MariaTime* newEndTime) {
	if (_endTime != NULL) {
		delete _endTime;
	}

	_endTime = newEndTime;
}

void MariaInputObject::setEditTime(MariaTime* newEditTime) {
	if (_editTime != NULL) {
		delete _editTime;
	}

	_editTime = newEditTime;
}

void MariaInputObject::setStateType(STATE_TYPE newStateType) {
	_stateType = newStateType;
}

string MariaInputObject::getOriginalInput() const {
	return _originalInput;
}

MariaInputObject::COMMAND_TYPE MariaInputObject::getCommandType() const {
	return _commandType;
}

string MariaInputObject::getTitle() const {
	return _title;
}

string MariaInputObject::getEditField() const {
	return _editField;
}

int MariaInputObject::getOptionID() const {
	return _optionID;
}

MariaTime* MariaInputObject::getStartTime() const {
	return new MariaTime(*_startTime);
}

MariaTime* MariaInputObject::getEndTime() const {
	return new MariaTime(*_endTime);
}

MariaTime* MariaInputObject::getEditTime() const {
	return new MariaTime(*_editTime);
}

STATE_TYPE MariaInputObject::getStateType() const {
	return _stateType;
}