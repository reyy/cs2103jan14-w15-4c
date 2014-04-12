#include "MariaText.h"

const string MariaText::EMPTY_STRING = "";
const string MariaText::TASK_ADDED_SUCESS = "Task '%s' has been added!";
const string MariaText::TASK_ADDED_ERROR = "There was a problem adding '%s'.";

const string MariaText::TASK_UPDATED_TITLE = "Ok, I have updated the title.";
const string MariaText::TASK_UPDATED_DESCRIPTION = "Ok, I have updated the description.";
const string MariaText::TASK_UPDATE_CONFLICT = "There are similar tasks, which one should I edit?";

const string MariaText::TASK_NOT_FOUND = "I couldn't find anything related. Try again.";

const string MariaText::SHOW_DATE = "This is what you have on %s.";
const string MariaText::SHOW_ALL = "Sure, here are all the tasks.";

const string MariaText::SEARCH_RESULT = "This is the result of the search for '%s'.";

string MariaText::format(string formatString, ...) {
	char buffer[256];
	va_list args;
	va_start (args, formatString);
	vsprintf (buffer,formatString.c_str(), args);
	va_end (args);
	return string(buffer);
}