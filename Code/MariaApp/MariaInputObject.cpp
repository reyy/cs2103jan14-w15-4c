#include "MariaInputObject.h"

MariaInputObject::MariaInputObject() {
	_commandType = CommandInvalid;
	_addType = AddNone;
	_editType = EditNone;

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

void MariaInputObject::setValidity(bool valid) {
	_validInput = valid;
}

void MariaInputObject::setOriginalInput(string originalInput) {
	_originalInput = originalInput;
}

void MariaInputObject::setCommandType(CommandType newCommandType) {
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
	_startTime = newStartTime;
}

void MariaInputObject::setEndTime(MariaTime* newEndTime) {
	_endTime = newEndTime;
}

void MariaInputObject::setEditTime(MariaTime* newEditTime) {
	_editTime = newEditTime;
}

void MariaInputObject::setAddType(AddType newAddType) {
	_addType = newAddType;
}

void MariaInputObject::setEditType(EditType newEditType) {
	_editType = newEditType;
}

void MariaInputObject::setStateType(MariaStateManager::STATE_TYPE newStateType) {
	_stateType = newStateType;
}

bool MariaInputObject::isValid() const {
	return _validInput;
}

string MariaInputObject::getOriginalInput() const {
	return _originalInput;
}

MariaInputObject::CommandType MariaInputObject::getCommandType() const {
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
	return _startTime;
}

MariaTime* MariaInputObject::getEndTime() const {
	return _endTime;
}

MariaTime* MariaInputObject::getEditTime() const {
	return _editTime;
}

MariaInputObject::AddType MariaInputObject::getAddType() const {
	return _addType;
}

MariaInputObject::EditType MariaInputObject::getEditType() const {
	return _editType;
}

MariaStateManager::STATE_TYPE MariaInputObject::getStateType() const {
	return _stateType;
}