//@author A0111821X
#pragma once

#include "MariaTime.h"

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
	} CommandType;

	MariaInputObject(void);
	~MariaInputObject(void);

	string			getOriginalInput() const;
	CommandType		getCommandType() const;
	string			getTitle() const;
	string			getEditField() const;
	int				getOptionID() const;
	MariaTime* 		getStartTime() const;
	MariaTime* 		getEndTime() const;
	MariaTime* 		getEditTime() const;

	void setOriginalInput(string originalInput);
	void setCommandType(CommandType newCommandType);
	void setTitle(string newTitle);
	void setEditField(string newEditField);
	void setOptionID(int newOptionID);
	void setStartTime(MariaTime* newStartTime);
	void setEndTime(MariaTime* newEndTime);
	void setEditTime(MariaTime* newEditTime);

private:
	CommandType		commandType;

	string			originalInput;
	string			title;
	string			editField;
	int				optionID;
	MariaTime* 		startTime;
	MariaTime* 		endTime;
	MariaTime* 		editTime;
};