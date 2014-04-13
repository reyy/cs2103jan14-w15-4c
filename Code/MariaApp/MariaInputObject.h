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
		DELETE_ALL,
		UNDO,
		GO_HOME,
		GO_HELP,
		GO_CREDITS,
		PAGE_UP,
		PAGE_DOWN,
		PAGE_LEFT,
		PAGE_RIGHT,
		EXIT,
	} COMMAND_TYPE;

	MariaInputObject();
	~MariaInputObject();

	void setOriginalInput(string originalInput);
	void setCOMMAND_TYPE(COMMAND_TYPE newCOMMAND_TYPE);
	void setTitle(string newTitle);
	void setEditField(string newEditField);
	void setOptionID(int newOptionID);
	void setStartTime(MariaTime* newStartTime);
	void setEndTime(MariaTime* newEndTime);
	void setEditTime(MariaTime* newEditTime);
	void setSTATE_TYPE(STATE_TYPE newSTATE_TYPE);

	string getOriginalInput() const;
	COMMAND_TYPE getCOMMAND_TYPE() const;
	string getTitle() const;
	string getEditField() const;
	int getOptionID() const;
	MariaTime* getStartTime() const;
	MariaTime* getEndTime() const;
	MariaTime* getEditTime() const;
	STATE_TYPE getSTATE_TYPE() const;

private:
	COMMAND_TYPE _COMMAND_TYPE;
	STATE_TYPE _STATE_TYPE;

	string _originalInput;
	string _title;
	string _editField;
	int _optionID;
	MariaTime* _startTime;
	MariaTime* _endTime;
	MariaTime* _editTime;
};