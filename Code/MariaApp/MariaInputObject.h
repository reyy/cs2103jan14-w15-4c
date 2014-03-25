#pragma once

#include "MariaTime.h"
#include "MariaStateManager.h"

class MariaInputObject {
public:
	typedef enum {
		INVALID,
		ADD,
		ADD_FLOATING,
		ADD_DEADLINE,
		ADD_TIMED,
		EDIT,											//Jay: On logic side, it suppose to tell it to output to user. "What would you like to edit?" which is still in Home State.
		EDIT_TASK,										//Jay: Newly added. To allow users edit task in edit state. (edit Apple) No time keyed.
		EDIT_TITLE,
		EDIT_START_TIME,
		EDIT_END_TIME,
		EDIT_CONFLICTED_TITLE,							//Jay: Newly added. To differentiate from non conflicted.
		EDIT_CONFLICTED_START_TIME,						//Jay: Newly added. To differentiate from non conflicted.
		EDIT_CONFLICTED_END_TIME,						//Jay: Newly added. To differentiate from non conflicted.						
		SHOW,
		SHOW_TASK,										//Jay: Newly added. Show individual task.
		SHOW_CONFLICTED_TASK,							//Jay: Newly added. Going to conflict state to show individual task. 
		SHOW_DATE,
		SHOW_DATE_RANGE,
		SHOW_ALL,
		MARK_DONE,
		MARK_UNDONE,
		MARK_CONFLICTED_DONE,							//Jay: Newly added. To differentiate from non conflicted.
		MARK_CONFLICTED_UNDONE,							//Jay: Newly added. To differentiate from non conflicted.
		DELETE_TASK,
		DELETE_CONFLICTED_TASK,							//Jay: Newly added. To differentiate from non conflicted.
		DELETE_ALL,
		UNDO,
		GO_ADD,											//Jay: Pending removal. There is no add state.
		GO_EDIT,										//Jay: Pending removal. There is already a default 'EDIT' .
		GO_DELETE,										//Jay: Pending removal. There is no add state.
		GO_HOME,
		GO_SETTINGS,
		GO_UP,											//Jay: Pending rename. GO in these cases refers to a change in state. Up only tells logic to call a function. Perhaps PRESS_UP?
		GO_DOWN,										//Jay: Pending removal. GO in these cases refers to a change in state. Up only tells logic to call a function. Perhaps PRESS_DOWN?
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
	void setStateType(MariaStateManager::STATE_TYPE newStateType);

	string getOriginalInput() const;
	COMMAND_TYPE getCommandType() const;
	string getTitle() const;
	string getEditField() const;
	int getOptionID() const;
	MariaTime* getStartTime() const;
	MariaTime* getEndTime() const;
	MariaTime* getEditTime() const;
	MariaStateManager::STATE_TYPE getStateType() const;

private:
	COMMAND_TYPE _commandType;
	MariaStateManager::STATE_TYPE _stateType;

	string _originalInput;
	string _title;
	string _editField;
	int _optionID;
	MariaTime* _startTime;
	MariaTime* _endTime;
	MariaTime* _editTime;
};