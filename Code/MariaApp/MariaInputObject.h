#pragma once

#include "MariaTime.h"
#include "MariaUIStateType.h"

class MariaInputObject {
public:
	typedef enum {
		INVALID,
		ADD,
		ADD_FLOATING,
		ADD_DEADLINE,
		ADD_TIMED,
		EDIT,
		EDIT_TITLE,
		EDIT_START_TIME,
		EDIT_END_TIME,
		EDIT_DESCRIPTION,
		SHOW,
		SHOW_DATE,
		SHOW_DATE_RANGE,
		SHOW_ALL,
		SEARCH,
		MARK_DONE,
		MARK_UNDONE,
		DELETE_TASK,
		DELETE_ALL,		//Pending removal
		UNDO,
		GO_HOME,
		GO_SETTINGS,
		GO_HELP,
		GO_CREDITS,
		PAGE_UP,
		PAGE_DOWN,
		EXIT,
	} COMMAND_TYPE;

	MariaInputObject();
	~MariaInputObject();

	void setOriginalInput(string originalInput);
	void setCommandType(COMMAND_TYPE newCommandType);
	void setTitle(string newTitle);
	void setEditField(string newEditField);
	void setOptionID(int newOptionID);
	void setStartTime(MariaTime* newStartTime);
	void setEndTime(MariaTime* newEndTime);
	void setEditTime(MariaTime* newEditTime);
	void setStateType(STATE_TYPE newStateType);

	string getOriginalInput() const;
	COMMAND_TYPE getCommandType() const;
	string getTitle() const;
	string getEditField() const;
	int getOptionID() const;
	MariaTime* getStartTime() const;
	MariaTime* getEndTime() const;
	MariaTime* getEditTime() const;
	STATE_TYPE getStateType() const;

private:
	COMMAND_TYPE _commandType;
	STATE_TYPE _stateType;

	string _originalInput;
	string _title;
	string _editField;
	int _optionID;
	MariaTime* _startTime;
	MariaTime* _endTime;
	MariaTime* _editTime;
};