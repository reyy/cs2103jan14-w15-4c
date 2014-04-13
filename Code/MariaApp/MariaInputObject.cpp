#include "MariaInputObject.h"

MariaInputObject::MariaInputObject() {
	_COMMAND_TYPE = INVALID;

	_startTime = NULL;
	_endTime = NULL;
	_editTime = NULL;
}

MariaInputObject::~MariaInputObject() {
	if (_startTime != NULL) {
		delete _startTime;
		_startTime = NULL;
	}

	if (_endTime != NULL) {
		delete _endTime;
		_endTime = NULL;
	}

	if (_editTime != NULL) {
		delete _editTime;
		_editTime = NULL;
	}
}

void MariaInputObject::setOriginalInput(string originalInput) {
	_originalInput = originalInput;
}

void MariaInputObject::setCOMMAND_TYPE(COMMAND_TYPE newCOMMAND_TYPE) {
	_COMMAND_TYPE = newCOMMAND_TYPE;
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

void MariaInputObject::setSTATE_TYPE(STATE_TYPE newSTATE_TYPE) {
	_STATE_TYPE = newSTATE_TYPE;
}

string MariaInputObject::getOriginalInput() const {
	return _originalInput;
}

MariaInputObject::COMMAND_TYPE MariaInputObject::getCOMMAND_TYPE() const {
	return _COMMAND_TYPE;
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

STATE_TYPE MariaInputObject::getSTATE_TYPE() const {
	return _STATE_TYPE;
}