#include "MariaText.h"

const string MariaText::EMPTY_STRING = "";
const string MariaText::TASK_ADDED_SUCCESS = "Task '%s' has been added!";
const string MariaText::TASK_ADDED_ERROR = "There was a problem adding '%s'.";

const string MariaText::TASK_UPDATED_TITLE = "Ok, I have updated the title.";
const string MariaText::TASK_UPDATED_TIME = "Consider it done!";
const string MariaText::TASK_UPDATED_DESCRIPTION = "Ok, I have updated the description.";
const string MariaText::TASK_UPDATE_CONFLICT = "There are similar tasks, which one should I edit?";

const exception MariaText::TASK_UPDATE_TYPE_ERROR = exception("Sorry, please recreate the activity to insert time.");
const exception MariaText::TASK_UPDATE_START_TIME_ERROR = exception("Sorry, but the new start time is after the end time.");
const exception MariaText::TASK_UPDATE_END_TIME_ERROR = exception("Sorry, but the new end time is before the start time.");

const string MariaText::TASK_NOT_FOUND = "I couldn't find anything related. Try again.";

const string MariaText::TASK_DELETED = "'%s' has been deleted!";
const string MariaText::TASK_RESOLVED_CONFLICT = "Resolved! Anything else?";

const string MariaText::TASK_MARK_COMPLETE = "'%s' has been marked as completed!";
const string MariaText::TASK_UNMARK_COMPLETE = "'%s' has been marked as incomplete!";

const string MariaText::SHOW_DATE = "This is what you have on %s.";
const string MariaText::SHOW_ALL = "Sure, here are all the tasks.";
const string MariaText::SHOW_RANGE = "%s to %s.";
const string MariaText::SEARCH_RESULT = "This is the result of the search for '%s'.";

const string MariaText::UNDO_SUCCESS = "Undo was successful!";
const string MariaText::UNDO_EMPTY = "Nothing to undo.";
const string MariaText::UNDO_ERROR = "Cannot perform Undo here.";

const string MariaText::HOME = "How can I help you?";
const string MariaText::LOADING = "Loading";
const string MariaText::EXIT = "Good Bye!";

const string MariaText::UP = "Going up.";
const string MariaText::UP_ERROR = "There are no more items up there.";
const string MariaText::DOWN = "Going down.";
const string MariaText::DOWN_ERROR = "There are no more items down there.";

const exception MariaText::IGNORE_EXCEPTION = exception("");

string MariaText::format(string formatString, ...) {
	char buffer[BUFFER_SIZE];
	va_list args;
	va_start (args, formatString);
	vsprintf (buffer, formatString.c_str(), args);
	va_end (args);
	return string(buffer);
}