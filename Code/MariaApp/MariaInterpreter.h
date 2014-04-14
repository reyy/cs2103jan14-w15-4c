//@author A0111821X
#pragma once

#include <string>
#include <map>
#include <sstream>
#include <algorithm>
#include <functional>
#include <regex>
#include <exception>
#include <cassert>
#include "MariaInputObject.h"
#include "MariaUIStateType.h"
#include "MariaMacros.h"
using namespace std;

class MariaInterpreter {
public:
	// Warning Messages
	static const string MESSAGE_BLANK;
	static const string MESSAGE_INVALID_COMMAND;
	static const string MESSAGE_INVALID_COMMAND_FORMAT;
	static const string MESSAGE_INVALID_COMMAND_LOCATION;
	static const string MESSAGE_INVALID_OPTION;
	static const string MESSAGE_INVALID_DATE_TIME;
	static const string MESSAGE_NO_ACTIVITY_TITLE;
	static const string MESSAGE_NO_ACTIVITY_TITLE_EDIT;
	static const string MESSAGE_NO_DESCRIPTION;
	static const string MESSAGE_NO_INPUT;
	static const string MESSAGE_NO_OPTION;

	MariaInterpreter(map<string, MariaInputObject::CommandType>* inputCommandList = NULL);
	~MariaInterpreter(void);

	bool checkValidCommand(string input);
	MariaInputObject* parseInput(string input, StateType currentState = StateType::HOME);

private:
	map<string, MariaInputObject::CommandType>* commandKeywordList;

	// Actual commands
	void parseAdd(string input, MariaInputObject* inputObject, StateType currentState);
	void parseAddDeadlineTask(string input, MariaInputObject* inputObject);
	void parseAddTimedTask(string input, MariaInputObject* inputObject);
	void parseEdit(string input, MariaInputObject* inputObject, StateType currentState);
	void parseEditType(string input, MariaInputObject* inputObject, int changeTitlePos, int changeStartPos, int changeEndPos, int changeDescPos);
	void parseEditTitle(string input, MariaInputObject* inputObject);
	void parseEditStartTime(string input, MariaInputObject* inputObject);
	void parseEditEndTime(string input, MariaInputObject* inputObject);
	void parseEditDescription(string input, MariaInputObject* inputObject);
	void parseShow(string input, MariaInputObject* inputObject, StateType currentState);
	void parseShowDateFormat(string input, MariaInputObject* inputObject);
	void parseShowDateRange(string input, MariaInputObject* inputObject);
	void parseSearch(string input, MariaInputObject* inputObject, StateType currentState);
	void parseDelete(string input, MariaInputObject* inputObject, StateType currentState);
	void parseMarkDone(string input, MariaInputObject* inputObject, StateType currentState);
	void parseMarkUndone(string input, MariaInputObject* inputObject, StateType currentState);

	// Parsing of date and time string into a MariaTime object.
	void parseTime(string input, int& hour, int& min);
	MariaTime* parseDateTimeString(vector<string> tokenizedInput);

	// Regular expressions for format checks.
	static const string EXPRESSION_DATE_FORMAT;
	static const string EXPRESSION_DAYS_OF_WEEK;
	static const string EXPRESSION_DAYS_OF_WEEK_INDIVIDUAL[7];
	static const string EXPRESSION_MONTHS_OF_YEAR;
	static const string EXPRESSION_MONTHS_OF_YEAR_INDIVIDUAL[12];
	static const string EXPRESSION_TODAY;
	static const string EXPRESSION_TOMORROW;
	static const string EXPRESSION_TIME_FORMAT;

	static const string MODIFIER_ALL_TASKS;

	// Delimiters that define how a text is seperated.
	static const string DELIMITER_ADD_DEADLINE_TASK;
	static const string DELIMITER_ADD_TIMED_TASK_START;
	static const string DELIMITER_ADD_TIMED_TASK_END;
	static const string DELIMITER_EDIT_TITLE;
	static const string DELIMITER_EDIT_START_TIME;
	static const string DELIMITER_EDIT_END_TIME;
	static const string DELIMITER_EDIT_DESCRIPTION;
	static const string DELIMITER_SHOW_DATE_RANGE_START;
	static const string DELIMITER_SHOW_DATE_RANGE_END;

	// Checks if the string passed in is in accordance to the expression defined above.
	bool hasDate(string text);
	bool hasTime(string text);
	bool hasDateTime(string text);
	bool hasDateFormat(string text);
	bool hasDayOfWeek(string text);
	bool hasMonth(string text);
	bool hasToday(string text);
	bool hasTomorrow(string text);

	// Helper functions
	bool checkValidTime(int hour, int min);
	string extractFromBackOfString(string text, string delimiter, int& delimiterPos);
	int getDayOfWeek(string text);
	int getLastDayOfMonth(int year, int month);
	int getMonth(string text);
	bool isInteger(string text);
	bool isStringContain(string text, string expr, bool ignoreCasing = true);
	bool isStringEqual(string text, string expr, bool ignoreCasing = true);
	string lowercaseString(string text);
	void removeTokens(vector<string> &input, int startPos, int endPos);
	string stitchString(vector<string> token, int startPos, int endPos, string delimiter = " ");
	vector<string> tokenizeString(string inputString, char delimiter = ' ');
	string trimWhiteSpaceLeft(string text);
	string trimWhiteSpaceRight(string text);
	string trimWhiteSpace(string text);
};