#pragma once

#include "MariaTime.h"

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
	void setConflictID(int newConflictID);
	void setStartTime(MariaTime* newStartTime);
	void setEndTime(MariaTime* newEndTime);
	void setAddType(AddType newAddType);
	void setEditType(EditType newEditType);

	bool isValid() const;
	string getOriginalInput() const;
	CommandType getCommandType() const;
	string getTitle() const;
	string getEditField() const;
	int getConflictID() const;
	MariaTime* getStartTime() const;
	MariaTime* getEndTime() const;
	AddType getAddType() const;
	EditType getEditType() const;

private:
	CommandType _commandType;
	AddType _addType;
	EditType _editType;

	bool _validInput;
	string _originalInput;
	string _title;
	string _editField;
	int _conflictID;
	MariaTime* _startTime;
	MariaTime* _endTime;
};