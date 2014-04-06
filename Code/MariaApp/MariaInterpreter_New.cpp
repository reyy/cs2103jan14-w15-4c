#include "MariaInterpreter_New.h"

const string MariaInterpreter_New::MESSAGE_INVALID_COMMAND = "Invalid command detected.";
const string MariaInterpreter_New::MESSAGE_NO_ACTIVITY_TITLE = "No activity title detected.";
const string MariaInterpreter_New::MESSAGE_NO_INPUT = "No input detected.";

MariaInterpreter_New::MariaInterpreter_New(map<string, MariaInputObject::COMMAND_TYPE>* inputCommandList) {
	commandKeywordList = inputCommandList;
	if (commandKeywordList == NULL) {
		commandKeywordList = new map<string, MariaInputObject::COMMAND_TYPE>();
	}

	//TODO: Put this into an ini file.
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("add", MariaInputObject::COMMAND_TYPE::ADD));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("create", MariaInputObject::COMMAND_TYPE::ADD));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("edit", MariaInputObject::COMMAND_TYPE::EDIT));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("update", MariaInputObject::COMMAND_TYPE::EDIT));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("show", MariaInputObject::COMMAND_TYPE::SHOW));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("view", MariaInputObject::COMMAND_TYPE::SHOW));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("complete", MariaInputObject::COMMAND_TYPE::MARK_DONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("done", MariaInputObject::COMMAND_TYPE::MARK_DONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("incomplete", MariaInputObject::COMMAND_TYPE::MARK_UNDONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("uncomplete", MariaInputObject::COMMAND_TYPE::MARK_UNDONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("undone", MariaInputObject::COMMAND_TYPE::MARK_UNDONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("delete", MariaInputObject::COMMAND_TYPE::DELETE_TASK));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("remove", MariaInputObject::COMMAND_TYPE::DELETE_TASK));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("clear", MariaInputObject::COMMAND_TYPE::DELETE_ALL));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("undo", MariaInputObject::COMMAND_TYPE::UNDO));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("home", MariaInputObject::COMMAND_TYPE::GO_HOME));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("settings", MariaInputObject::COMMAND_TYPE::GO_SETTINGS));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("up", MariaInputObject::COMMAND_TYPE::PAGE_UP));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("down", MariaInputObject::COMMAND_TYPE::PAGE_DOWN));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("exit", MariaInputObject::COMMAND_TYPE::EXIT));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("quit", MariaInputObject::COMMAND_TYPE::EXIT));
}

MariaInterpreter_New::~MariaInterpreter_New(void) {
	commandKeywordList->clear();
	SAFE_DELETE(commandKeywordList);
}

MariaInputObject* MariaInterpreter_New::parseInput(string input, STATE_TYPE currentState) {
	MariaInputObject* toReturn = NULL;

	input = trimWhiteSpace(input);

	if (input.size() == 0) {
		throw exception(MESSAGE_NO_INPUT.c_str());
	}

	toReturn = new MariaInputObject();
	toReturn->setOriginalInput(input);

	// First I want to remove the first word to see if it is a valid command keyword.
	vector<string> tokenizedInput = tokenizeString(input);
	map<string, MariaInputObject::COMMAND_TYPE>::iterator commandKeyword;
	commandKeyword = commandKeywordList->find(lowercaseString(tokenizedInput[0]));

	if (commandKeyword == commandKeywordList->end()) {
		SAFE_DELETE(toReturn);
		//TODO: Add the actual keyword that went wrong.
		throw exception(MESSAGE_INVALID_COMMAND.c_str());
	}

	// Now remove that command keyword with the trailing space.
	toReturn->setCommandType(commandKeyword->second);
	removeTokens(tokenizedInput, 0, 1);
	input = stitchString(tokenizedInput, 0, tokenizedInput.size());

	switch (commandKeyword->second) {
	case MariaInputObject::COMMAND_TYPE::ADD:
		parseAdd(input, toReturn);
		break;

	case MariaInputObject::COMMAND_TYPE::SHOW:
		parseShow(input, toReturn);
		break;

	case MariaInputObject::COMMAND_TYPE::DELETE_TASK:
		parseDelete(input, toReturn);
		break;
	}

	return toReturn;
}

void MariaInterpreter_New::parseAdd(string input, MariaInputObject* inputObject) {
	assert(inputObject != NULL);

	if (hasDateTime(input)) {
		int apple = 5;
	} else {
		inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::ADD_FLOATING);
		inputObject->setTitle(input);
	}
}

void MariaInterpreter_New::parseShow(string input, MariaInputObject* inputObject) {
	assert(inputObject != NULL);

	if (input.size() == 0) {
		// Show today.
		inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_DATE);
		inputObject->setEndTime(new MariaTime(MariaTime::getCurrentTime()));
	} else if (isStringEqual(input, "all")) {
		inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_ALL);
	} else if (isStringEqual(input, "today")) {
		inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_DATE);
		inputObject->setEndTime(new MariaTime(MariaTime::getCurrentTime()));
	} else if (isStringEqual(input, "tomorrow")) {
		inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_DATE);
		inputObject->setEndTime(new MariaTime(MariaTime::getCurrentTime().getYear(), MariaTime::getCurrentTime().getMonth(), MariaTime::getCurrentTime().getDay()+1));
	} else if (hasDate(input)) {
		// Need to check if there are 1 or more dates.
	} else {
	}
}

void MariaInterpreter_New::parseDelete(string input, MariaInputObject* inputObject) {
	assert(inputObject != NULL);

	if (input.size() == 0) {
		throw exception(MESSAGE_NO_ACTIVITY_TITLE.c_str());
	} else if (isStringEqual(input, "all")) {
		inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::DELETE_ALL);
	} else {
		inputObject->setTitle(input);
	}
}

bool MariaInterpreter_New::hasDate(string text) {
	bool isDatePresent = hasDateFormat(text);
	bool isDayOfWeekPresent = hasDayOfWeek(text);
	bool isTodayPresent = hasToday(text);
	bool isTomorrowPresent = hasTomorrow(text);

	return (isDatePresent || isDayOfWeekPresent || isTodayPresent || isTomorrowPresent);
}

bool MariaInterpreter_New::hasDateTime(string text) {
	bool isDatePresent = hasDateFormat(text);
	bool isDayOfWeekPresent = hasDayOfWeek(text);
	bool isTodayPresent = hasToday(text);
	bool isTomorrowPresent = hasTomorrow(text);

	//TODO: Add Time.

	return (isDatePresent || isDayOfWeekPresent || isTodayPresent || isTomorrowPresent);
}

bool MariaInterpreter_New::hasDateFormat(string text) {
	//TODO: May need further revision.
	regex dateExpression("(by|from|to)[ ](^(0?[1-9]|[12][0-9]|3[01])[-/.](0?[1-9]|1[012])[-/.](19|20)?[0-9][0-9]$)", regex_constants::icase);

	return regex_search(text, dateExpression);
}

bool MariaInterpreter_New::hasDayOfWeek(string text) {
	regex dayOfWeekExpression("(by|from|to)[ ](next )?mon(day)?|tues?(day)?|wed(nesday)?|thur?s?(day)?|fri(day)?|sat(urday)?|(sun)?day", regex_constants::icase);

	return regex_search(text, dayOfWeekExpression);
}

bool MariaInterpreter_New::hasToday(string text) {
	regex todayExpression("(by|from|to)[ ]today", regex_constants::icase);

	return regex_search(text, todayExpression);
}

bool MariaInterpreter_New::hasTomorrow(string text) {
	regex tomorrowExpression("(by|from|to)[ ]tomorrow", regex_constants::icase);

	return regex_search(text, tomorrowExpression);
}

bool MariaInterpreter_New::isStringEqual(string text, string expr, bool ignoreCasing) {
	regex* expression;

	if (ignoreCasing) {
		expression = new regex(expr, regex_constants::icase);
	} else {
		expression = new regex(expr);
	}

	bool result = regex_match(text, *expression);

	SAFE_DELETE(expression);

	return result;
}

string MariaInterpreter_New::lowercaseString(string text) {
	string toReturn = "";

	for (int i = 0; i < text.size(); i++ ) {
		toReturn += tolower(text[i]);
	}

	return toReturn;
}

void MariaInterpreter_New::removeTokens(vector<string> &input, int startPos, int endPos) {
	int lengthToRemove = endPos - startPos;

	for (int i = 0; i < lengthToRemove; i++) {
		input.erase(input.begin()+startPos);
	}
}

string MariaInterpreter_New::stitchString(vector<string> token, int startPos, int endPos, string delimiter) {
	string toReturn = "";

	for (int i = startPos; i < endPos; i++) {
		toReturn += token[i];

		if (i != endPos - 1) {
			toReturn += delimiter;
		}
	}

	return toReturn;
}

vector<string> MariaInterpreter_New::tokenizeString(string inputString, char delimiter) {
	vector<string> tokens;
	stringstream ss(inputString);
	string token;

	while (getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

string MariaInterpreter_New::trimWhiteSpaceLeft(string text) {
	text.erase(text.begin(), find_if(text.begin(), text.end(), not1(ptr_fun<int, int>(isspace))));
	return text;
}

string MariaInterpreter_New::trimWhiteSpaceRight(string text) {
	text.erase(find_if(text.rbegin(), text.rend(), not1(ptr_fun<int, int>(isspace))).base(), text.end());
	return text;
}

string MariaInterpreter_New::trimWhiteSpace(string text) {
	return trimWhiteSpaceLeft(trimWhiteSpaceRight(text));
}