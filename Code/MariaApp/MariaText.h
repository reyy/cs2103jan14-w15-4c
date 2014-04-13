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
	static const string TASK_UPDATE_CONFLICT;

	static const string TASK_DELETE;
	static const string TASK_DELETE_CONFLICT;

	static const string TASK_NOT_FOUND;

	static const string SHOW_DATE;
	static const string SHOW_ALL;

	static const string SEARCH_RESULT;

	static const int BUFFER = 256;

	static string format(string formatString, ... );
private:
	MariaText(void);
};

