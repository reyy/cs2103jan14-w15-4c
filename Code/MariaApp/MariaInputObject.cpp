//@author A0111821X
#include "MariaMacros.h"
#include "MariaInputObject.h"

MariaInputObject::MariaInputObject() {
	commandType = INVALID;

	startTime = NULL;
	endTime = NULL;
	editTime = NULL;
}

MariaInputObject::~MariaInputObject() {
	SAFE_DELETE(startTime);
	SAFE_DELETE(endTime);
	SAFE_DELETE(editTime);
}

void MariaInputObject::setOriginalInput(string originalInput) {
	this->originalInput = originalInput;
}

void MariaInputObject::setCommandType(CommandType newCommandType) {
	commandType = newCommandType;
}

void MariaInputObject::setTitle(string newTitle) {
	title = newTitle;
}

void MariaInputObject::setEditField(string newEditField) {
	editField = newEditField;
}

void MariaInputObject::setOptionID(int newOptionID) {
	optionID = newOptionID;
}

void MariaInputObject::setStartTime(MariaTime* newStartTime) {
	SAFE_DELETE(startTime);

	startTime = newStartTime;
}

void MariaInputObject::setEndTime(MariaTime* newEndTime) {
	SAFE_DELETE(endTime);

	endTime = newEndTime;
}

void MariaInputObject::setEditTime(MariaTime* newEditTime) {
	SAFE_DELETE(editTime);

	editTime = newEditTime;
}

string MariaInputObject::getOriginalInput() const {
	return originalInput;
}

MariaInputObject::CommandType MariaInputObject::getCommandType() const {
	return commandType;
}

string MariaInputObject::getTitle() const {
	return title;
}

string MariaInputObject::getEditField() const {
	return editField;
}

int MariaInputObject::getOptionID() const {
	return optionID;
}

MariaTime* MariaInputObject::getStartTime() const {
	return new MariaTime(*startTime);
}

MariaTime* MariaInputObject::getEndTime() const {
	return new MariaTime(*endTime);
}

MariaTime* MariaInputObject::getEditTime() const {
	return new MariaTime(*editTime);
}