#pragma once

#include "MariaTime.h"
#include "MariaStateManager.h"

class MariaInputObject {
public:
	typedef enum {
		CommandInvalid,
		CommandAdd,
		CommandEdit,
		CommandShowAll,
		CommandDelete,
		CommandDeleteAll,
		CommandGoToHome,
		CommandGoUp,
		CommandGoDown,
		CommandExit,
	} CommandType;

	typedef enum {
		AddNone, 
		AddFloating, 
		AddDeadline, 
		AddTimed, 
	} AddType;

	typedef enum {
		EditNone, 
		EditTitle, 
		EditStartTime, 
		EditEndTime, 
	} EditType;

	MariaInputObject();
	~MariaInputObject();

	void setValidity(bool valid);
	void setOriginalInput(string originalInput);
	void setCommandType(CommandType newCommandType);
	void setTitle(string newTitle);
	void setEditField(string newEditField);
	void setOptionID(int newOptionID);
	void setStartTime(MariaTime* newStartTime);
	void setEndTime(MariaTime* newEndTime);
	void setAddType(AddType newAddType);
	void setEditType(EditType newEditType);
	void setStateType(MariaStateManager::STATE_TYPE newStateType);

	bool isValid() const;
	string getOriginalInput() const;
	CommandType getCommandType() const;
	string getTitle() const;
	string getEditField() const;
	int getOptionID() const;
	MariaTime* getStartTime() const;
	MariaTime* getEndTime() const;
	AddType getAddType() const;
	EditType getEditType() const;
	MariaStateManager::STATE_TYPE getStateType() const;

private:
	CommandType _commandType;
	AddType _addType;
	EditType _editType;
	MariaStateManager::STATE_TYPE _stateType;

	bool _validInput;
	string _originalInput;
	string _title;
	string _editField;
	int _optionID;
	MariaTime* _startTime;
	MariaTime* _endTime;
};