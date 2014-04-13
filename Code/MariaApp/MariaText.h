#pragma once
#include<string>
#include<memory>
#include <stdarg.h>
using namespace std;

class MariaText
{
public:
	
	static const string EMPTY_STRING;
	static const string TASK_ADDED_SUCESS;
	static const string TASK_ADDED_ERROR;

	static const string TASK_UPDATED_TITLE;
	static const string TASK_UPDATED_DESCRIPTION;
	static const string TASK_UPDATED_TIME;
	static const string TASK_UPDATE_CONFLICT;

	static const exception TASK_UPDATE_TYPE_ERROR;
	static const exception TASK_UPDATE_START_TIME_ERROR;
	static const exception TASK_UPDATE_END_TIME_ERROR;

	static const string TASK_DELETED;
	static const string TASK_RESOLVED_CONFLICT;
	
	static const string TASK_MARK_COMPLETE;
	static const string TASK_UNMARK_COMPLETE;

	static const string TASK_NOT_FOUND;

	static const string SHOW_DATE;
	static const string SHOW_RANGE;
	static const string SHOW_ALL;

	static const string UNDO_SUCCESS;
	static const string UNDO_EMPTY;
	static const string UNDO_ERROR;

	static const string SEARCH_RESULT;
	
	static const string HOME;
	static const string LOADING;
	static const string EXIT;

	static const string UP;
	static const string UP_ERROR;
	static const string DOWN;
	static const string DOWN_ERROR;

	static const exception IGNORE_EXCEPTION;
	static const int BUFFER_SIZE = 256;

	static string format(string formatString, ... );
private:
	MariaText(void);
};

