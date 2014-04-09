//@author A0111821X
#include "MariaInterpreter.h"

const string MariaInterpreter::MESSAGE_INVALID_COMMAND = "Invalid command detected.";
const string MariaInterpreter::MESSAGE_INVALID_COMMAND_FORMAT = "Invalid command format detected.";
const string MariaInterpreter::MESSAGE_INVALID_OPTION = "Invalid option.";
const string MariaInterpreter::MESSAGE_INVALID_DATE_TIME = "Invalid date/time detected.";
const string MariaInterpreter::MESSAGE_NO_ACTIVITY_TITLE = "No activity title detected.";
const string MariaInterpreter::MESSAGE_NO_ACTIVITY_TITLE_EDIT = "No new activity title detected.";
const string MariaInterpreter::MESSAGE_NO_INPUT = "No input detected.";
const string MariaInterpreter::MESSAGE_NO_OPTION = "No option selected.";

MariaInterpreter::MariaInterpreter(map<string, MariaInputObject::COMMAND_TYPE>* inputCommandList) {
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
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("search", MariaInputObject::COMMAND_TYPE::SEARCH));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("find", MariaInputObject::COMMAND_TYPE::SEARCH));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("complete", MariaInputObject::COMMAND_TYPE::MARK_DONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("done", MariaInputObject::COMMAND_TYPE::MARK_DONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("incomplete", MariaInputObject::COMMAND_TYPE::MARK_UNDONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("uncomplete", MariaInputObject::COMMAND_TYPE::MARK_UNDONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("undone", MariaInputObject::COMMAND_TYPE::MARK_UNDONE));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("delete", MariaInputObject::COMMAND_TYPE::DELETE_TASK));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("remove", MariaInputObject::COMMAND_TYPE::DELETE_TASK));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("clear", MariaInputObject::COMMAND_TYPE::DELETE_ALL));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("undo", MariaInputObject::COMMAND_TYPE::UNDO));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("back", MariaInputObject::COMMAND_TYPE::GO_HOME));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("home", MariaInputObject::COMMAND_TYPE::GO_HOME));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("settings", MariaInputObject::COMMAND_TYPE::GO_SETTINGS));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("help", MariaInputObject::COMMAND_TYPE::GO_HELP));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("up", MariaInputObject::COMMAND_TYPE::PAGE_UP));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("down", MariaInputObject::COMMAND_TYPE::PAGE_DOWN));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("exit", MariaInputObject::COMMAND_TYPE::EXIT));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("quit", MariaInputObject::COMMAND_TYPE::EXIT));
}

MariaInterpreter::~MariaInterpreter(void) {
	commandKeywordList->clear();
	SAFE_DELETE(commandKeywordList);
}

bool MariaInterpreter::checkValidCommand(string input) {
	if (input.size() == 0) {
		return false;
	}

	vector<string> tokenizedInput = tokenizeString(input);
	map<string, MariaInputObject::COMMAND_TYPE>::iterator commandKeyword = commandKeywordList->find(lowercaseString(tokenizedInput[0]));

	if (commandKeyword == commandKeywordList->end()) {
		return false;
	}

	return true;
}

MariaInputObject* MariaInterpreter::parseInput(string input, STATE_TYPE currentState) {
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
		parseAdd(input, toReturn, currentState);
		break;

	case MariaInputObject::COMMAND_TYPE::EDIT:
		parseEdit(input, toReturn, currentState);
		break;

	case MariaInputObject::COMMAND_TYPE::SHOW:
		parseShow(input, toReturn, currentState);
		break;

	case MariaInputObject::COMMAND_TYPE::SEARCH:
		parseSearch(input, toReturn, currentState);
		break;

	case MariaInputObject::COMMAND_TYPE::DELETE_TASK:
		parseDelete(input, toReturn, currentState);
		break;

	case MariaInputObject::COMMAND_TYPE::MARK_DONE:
		parseMarkDone(input, toReturn, currentState);
		break;

	case MariaInputObject::COMMAND_TYPE::MARK_UNDONE:
		parseMarkUndone(input, toReturn, currentState);
		break;
	}

	return toReturn;
}

void MariaInterpreter::parseAdd(string input, MariaInputObject* inputObject, STATE_TYPE currentState) {
	assert(inputObject != NULL);

	if (input.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_NO_ACTIVITY_TITLE.c_str());
	}

	int dummyVar = 0;

	if (hasDateTime(input)) {
		// Check if the substring after the last by/from/to contains a valid date/time.
		if (hasDateTime(extractFromBackOfString(input, " from ", dummyVar)) && hasDateTime(extractFromBackOfString(input, " to ", dummyVar))) {
			parseAddTimedTask(input, inputObject);
			inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::ADD_TIMED);
		} else if (hasDateTime(extractFromBackOfString(input, " by ", dummyVar))) {
			parseAddDeadlineTask(input, inputObject);
			inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::ADD_DEADLINE);
		} else {
			// Something's wrong with the string but nevermind. We'll just put it as a floating task.
			inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::ADD_FLOATING);
			inputObject->setTitle(input);
		}
	} else {
		inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::ADD_FLOATING);
		inputObject->setTitle(input);
	}
}

void MariaInterpreter::parseAddDeadlineTask(string input, MariaInputObject* inputObject) {
	assert(inputObject != NULL);
	int delimiterPos = 0;
	string dateTimeString = extractFromBackOfString(input, " by ", delimiterPos);

	input = input.substr(0, delimiterPos);
	inputObject->setTitle(input);

	if (dateTimeString.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
	}

	dateTimeString = trimWhiteSpace(dateTimeString);
	vector<string> tokenizedDateTime = tokenizeString(dateTimeString);
	removeTokens(tokenizedDateTime, 0, 1);

	MariaTime* endTime;

	try {
		endTime = parseDateTimeString(tokenizedDateTime);
	} catch (exception& e) {
		SAFE_DELETE(inputObject);
		throw;
	}

	inputObject->setEndTime(endTime);
}

void MariaInterpreter::parseAddTimedTask(string input, MariaInputObject* inputObject) {
	assert(inputObject != NULL);

	////////
	// To //
	////////
	int delimiterPos = 0;
	string dateTimeString = extractFromBackOfString(input, " to ", delimiterPos);

	input = input.substr(0, delimiterPos);

	if (dateTimeString.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
	}

	dateTimeString = trimWhiteSpace(dateTimeString);
	vector<string> tokenizedDateTime = tokenizeString(dateTimeString);
	removeTokens(tokenizedDateTime, 0, 1);

	MariaTime* endTime;

	try {
		endTime = parseDateTimeString(tokenizedDateTime);
	} catch (exception& e) {
		SAFE_DELETE(inputObject);
		throw;
	}

	inputObject->setEndTime(endTime);

	//////////
	// From //
	//////////
	delimiterPos = 0;
	dateTimeString = extractFromBackOfString(input, " from ", delimiterPos);

	input = input.substr(0, delimiterPos);

	inputObject->setTitle(input);

	if (dateTimeString.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
	}

	dateTimeString = trimWhiteSpace(dateTimeString);
	tokenizedDateTime = tokenizeString(dateTimeString);
	removeTokens(tokenizedDateTime, 0, 1);

	MariaTime* startTime;

	try {
		startTime = parseDateTimeString(tokenizedDateTime);
	} catch (exception& e) {
		SAFE_DELETE(inputObject);
		throw;
	}

	inputObject->setStartTime(startTime);
}

void MariaInterpreter::parseEdit(string input, MariaInputObject* inputObject, STATE_TYPE currentState) {
	assert(inputObject != NULL);

	if (input.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_NO_ACTIVITY_TITLE.c_str());
	}

	vector<string> tokenizedInput = tokenizeString(input);
	// <task title> change title/start/end <editfield>
	// therefore 4 tokens minimum
	if (tokenizedInput.size() < 4) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_INVALID_COMMAND_FORMAT.c_str());
	}

	int changeTitlePos;
	int changeStartPos;
	int changeEndPos;
	int changeDescPos;
	int inputSize = input.size();
	string editFieldString;
	
	extractFromBackOfString(input, " change title ", changeTitlePos);
	extractFromBackOfString(input, " change start ", changeStartPos);
	extractFromBackOfString(input, " change end ", changeEndPos);
	extractFromBackOfString(input, " change description ", changeDescPos);

	switch (currentState) {
	case STATE_TYPE::CONFLICT:
		if (isInteger(tokenizedInput[0])) {
			inputObject->setOptionID(atoi(tokenizedInput[0].c_str()));

			// Carry on parsing.
			if (changeTitlePos != inputSize &&
				changeStartPos == inputSize &&
				changeEndPos == inputSize &&
				changeDescPos == inputSize) {
					parseEditTitle(input, inputObject);
			} else if (changeTitlePos == inputSize &&
				changeStartPos != inputSize &&
				changeEndPos == inputSize &&
				changeDescPos == inputSize) {
					parseEditStartTime(input, inputObject);
			} else if (changeTitlePos == inputSize &&
				changeStartPos == inputSize &&
				changeEndPos != inputSize &&
				changeDescPos == inputSize) {
					parseEditEndTime(input, inputObject);
			} else if (changeTitlePos == inputSize &&
				changeStartPos == inputSize &&
				changeEndPos == inputSize &&
				changeDescPos != inputSize) {
					parseEditDescription(input, inputObject);
			} else {
				SAFE_DELETE(inputObject);
				throw exception(MESSAGE_INVALID_COMMAND_FORMAT.c_str());
			}
		} else {
			throw exception(MESSAGE_INVALID_OPTION.c_str());
		}
		break;

	default:
		if (changeTitlePos != inputSize &&
			changeStartPos == inputSize &&
			changeEndPos == inputSize &&
			changeDescPos == inputSize) {
				parseEditTitle(input, inputObject);
		} else if (changeTitlePos == inputSize &&
			changeStartPos != inputSize &&
			changeEndPos == inputSize &&
			changeDescPos == inputSize) {
				parseEditStartTime(input, inputObject);
		} else if (changeTitlePos == inputSize &&
			changeStartPos == inputSize &&
			changeEndPos != inputSize &&
			changeDescPos == inputSize) {
				parseEditEndTime(input, inputObject);
		} else if (changeTitlePos == inputSize &&
			changeStartPos == inputSize &&
			changeEndPos == inputSize &&
			changeDescPos != inputSize) {
				parseEditDescription(input, inputObject);
		} else {
			SAFE_DELETE(inputObject);
			throw exception(MESSAGE_INVALID_COMMAND_FORMAT.c_str());
		}
		break;
	}
}

void MariaInterpreter::parseEditTitle(string input, MariaInputObject* inputObject) {
	vector<string> tokenizedInput = tokenizeString(input);
	int changeTitlePos;
	int inputSize = input.size();
	string editFieldString;
	
	extractFromBackOfString(input, " change title ", changeTitlePos);
	editFieldString = extractFromBackOfString(input, " change title ", changeTitlePos);
	input = input.substr(0, changeTitlePos);
	inputObject->setTitle(input);
	tokenizedInput = tokenizeString(editFieldString);
	editFieldString = stitchString(tokenizedInput, 3, tokenizedInput.size());

	if (editFieldString.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_NO_ACTIVITY_TITLE_EDIT.c_str());
	}
	inputObject->setEditField(editFieldString);
	inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::EDIT_TITLE);
}

void MariaInterpreter::parseEditStartTime(string input, MariaInputObject* inputObject) {
	int changeStartPos;
	string editFieldString = extractFromBackOfString(input, " change start ", changeStartPos);
	input = input.substr(0, changeStartPos);
	inputObject->setTitle(input);

	if (editFieldString.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
	}

	editFieldString = trimWhiteSpace(editFieldString);
	vector<string> tokenizedDateTime = tokenizeString(editFieldString);
	removeTokens(tokenizedDateTime, 0, 1);

	MariaTime* editTime;

	try {
		editTime = parseDateTimeString(tokenizedDateTime);
	} catch (exception& e) {
		SAFE_DELETE(inputObject);
		throw;
	}

	inputObject->setEditTime(editTime);
	inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::EDIT_START_TIME);
}

void MariaInterpreter::parseEditEndTime(string input, MariaInputObject* inputObject) {
	int changeEndPos;
	string editFieldString = extractFromBackOfString(input, " change end ", changeEndPos);
	input = input.substr(0, changeEndPos);
	inputObject->setTitle(input);

	if (editFieldString.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
	}

	editFieldString = trimWhiteSpace(editFieldString);
	vector<string> tokenizedDateTime = tokenizeString(editFieldString);
	removeTokens(tokenizedDateTime, 0, 1);

	MariaTime* editTime;

	try {
		editTime = parseDateTimeString(tokenizedDateTime);
	} catch (exception& e) {
		SAFE_DELETE(inputObject);
		throw;
	}

	inputObject->setEditTime(editTime);
	inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::EDIT_END_TIME);
}

void MariaInterpreter::parseEditDescription(string input, MariaInputObject* inputObject) {
	vector<string> tokenizedInput = tokenizeString(input);
	int changeDescPos;
	int inputSize = input.size();
	string editFieldString;
	
	extractFromBackOfString(input, " change description ", changeDescPos);
	editFieldString = extractFromBackOfString(input, " change description ", changeDescPos);
	input = input.substr(0, changeDescPos);
	inputObject->setTitle(input);
	tokenizedInput = tokenizeString(editFieldString);
	editFieldString = stitchString(tokenizedInput, 3, tokenizedInput.size());

	if (editFieldString.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_NO_ACTIVITY_TITLE_EDIT.c_str());
	}
	inputObject->setEditField(editFieldString);
	inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::EDIT_DESCRIPTION);
}

void MariaInterpreter::parseShow(string input, MariaInputObject* inputObject, STATE_TYPE currentState) {
	assert(inputObject != NULL);

	if (input.size() == 0) {
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
		if (isStringEqual(input, "(^(0?[1-9]|[12][0-9]|3[01])[-/](0?[1-9]|1[012])[-/](19|20)?[0-9][0-9]$)")) {
			int seperatorPos = 0;
			char seperatorArray[2] = { '/', '-' };

			for (int j = 0; j < 2; j++) {
				seperatorPos = input.find(seperatorArray[j]);

				if (seperatorPos != string::npos) {
					vector<string> workingList = tokenizeString(input, seperatorArray[j]);

					if (workingList.size() != 3) {
						SAFE_DELETE(inputObject);
						throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
					}

					int day = atoi(workingList[0].c_str());
					int month = atoi(workingList[1].c_str());
					int year = atoi(workingList[2].c_str());

					if (day <= 0) {
						day = 1;
					} else if (day >= 32) {
						day = 31;
					}

					if (month <= 0) {
						month = 1;
					} else if (month >= 13) {
						month = 12;
					}

					if (year < 100) {
						year += 2000;
					}

					break;
				}
			}
		} else if (isStringContain(input, "jan(uary)?|feb(ruary)?|mar(ch)?|apr(il)?|may|june?|july?|aug(ust)?|sept?(ember)?|oct(tober)?|nov(ember)?|dec(ember)?")) {
			int year = MariaTime::getCurrentTime().getYear();
			int month;
			int day;

			vector<string> workingList = tokenizeString(input);

			if (workingList.size() == 1) {
				// Just show month
				if (isStringEqual(workingList[0], "jan(uary)?|feb(ruary)?|mar(ch)?|apr(il)?|may|june?|july?|aug(ust)?|sept?(ember)?|oct(tober)?|nov(ember)?|dec(ember)?")) {
					month = getMonth(workingList[0]);
				} else {
					SAFE_DELETE(inputObject);
					throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
				}
				// Need a function to get last day of month...
				inputObject->setStartTime(new MariaTime(year, month, 1));
				inputObject->setEndTime(new MariaTime(year, month, getLastDayOfMonth(year, month)));
				inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_DATE_RANGE);
			} else if (workingList.size() == 2) {
				// Show date
				if (isStringEqual(workingList[1], "jan(uary)?|feb(ruary)?|mar(ch)?|apr(il)?|may|june?|july?|aug(ust)?|sept?(ember)?|oct(tober)?|nov(ember)?|dec(ember)?")) {
					month = getMonth(workingList[1]);
				} else {
					SAFE_DELETE(inputObject);
					throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
				}

				day = atoi(workingList[0].c_str());
				if (day <= 0) {
					day = 1;
				} else if (day >= 32) {
					day = 31;
				}

				inputObject->setEndTime(new MariaTime(year, month, day));
				inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_DATE);
			}

			// Check to see if the month is past today's date.
			// If it is, advance by one year.
			if (month < MariaTime::getCurrentTime().getMonth()) {
				year++;
			}

			/*// Check the previous token if it is a valid day.
			// If it isn't, just set it to 1st of whatever month this is.
			if (i >= 1 && isInteger(tokenizedDateTime[i-1])) {
				day = atoi(tokenizedDateTime[i-1].c_str());

				// Don't know if this check is necessary. Cause of maketime.
				if (day <= 0) {
					day = 1;
				} else if (day >= 32) {
					day = 31;
				}
				i--;
			} else {
				if (month == MariaTime::getCurrentTime().getMonth()) {
					year++;
				}
				day = 1;
			}*/
			inputObject->setEndTime(new MariaTime(year, month, day));
		} else if (isStringEqual(input, "mon(day)?|tues?(day)?|wed(nesday)?|thur?s?(day)?|fri(day)?|sat(urday)?|(sun)?day")) {
			int year = MariaTime::getCurrentTime().getYear();
			int month = MariaTime::getCurrentTime().getMonth();
			int day = MariaTime::getCurrentTime().getDay();

			// do that minus magic thingy
			int currentDayOfWeek = MariaTime::getCurrentTime().getDayWeek();
			int inputDayOfWeek = getDayOfWeek(input);
			int differenceInDays = inputDayOfWeek - currentDayOfWeek;

			if (differenceInDays < 0) {
				day += (7 - abs(differenceInDays));
			} else {
				day += differenceInDays;
			}

			// Also check if the preceding token is "next".
			// If it is, add a week to this day.
			inputObject->setEndTime(new MariaTime(year, month, day));
		}
	} else {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_INVALID_COMMAND.c_str());
	}
}

void MariaInterpreter::parseSearch(string input, MariaInputObject* inputObject, STATE_TYPE currentState) {
	assert(inputObject != NULL);

	if (input.size() == 0) {
		SAFE_DELETE(inputObject);
		throw exception(MESSAGE_NO_ACTIVITY_TITLE.c_str());
	} else {
		inputObject->setTitle(input);
	}
}

void MariaInterpreter::parseDelete(string input, MariaInputObject* inputObject, STATE_TYPE currentState) {
	assert(inputObject != NULL);

	switch (currentState) {
	case STATE_TYPE::CONFLICT:
		if (input.size() == 0) {
			SAFE_DELETE(inputObject);
			throw exception(MESSAGE_NO_OPTION.c_str());
		} else {
			if (isInteger(input)) {
				inputObject->setOptionID(atoi(input.c_str()));
			} else {
				throw exception(MESSAGE_INVALID_OPTION.c_str());
			}
		}
		break;

	default:
		if (input.size() == 0) {
			SAFE_DELETE(inputObject);
			throw exception(MESSAGE_NO_ACTIVITY_TITLE.c_str());
		} else if (isStringEqual(input, "all")) {
			inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::DELETE_ALL);
		} else {
			inputObject->setTitle(input);
		}
		break;
	}
}

void MariaInterpreter::parseMarkDone(string input, MariaInputObject* inputObject, STATE_TYPE currentState) {
	assert(inputObject != NULL);

	switch (currentState) {
	case STATE_TYPE::CONFLICT:
		if (input.size() == 0) {
			SAFE_DELETE(inputObject);
			throw exception(MESSAGE_NO_OPTION.c_str());
		} else {
			if (isInteger(input)) {
				inputObject->setOptionID(atoi(input.c_str()));
			} else {
				throw exception(MESSAGE_INVALID_OPTION.c_str());
			}
		}
		break;

	default:
		if (input.size() == 0) {
			SAFE_DELETE(inputObject);
			throw exception(MESSAGE_NO_ACTIVITY_TITLE.c_str());
		} else {
			inputObject->setTitle(input);
		}
		break;
	}
}

void MariaInterpreter::parseMarkUndone(string input, MariaInputObject* inputObject, STATE_TYPE currentState) {
	assert(inputObject != NULL);

	switch (currentState) {
	case STATE_TYPE::CONFLICT:
		if (input.size() == 0) {
			SAFE_DELETE(inputObject);
			throw exception(MESSAGE_NO_OPTION.c_str());
		} else {
			if (isInteger(input)) {
				inputObject->setOptionID(atoi(input.c_str()));
			} else {
				throw exception(MESSAGE_INVALID_OPTION.c_str());
			}
		}
		break;

	default:
		if (input.size() == 0) {
			SAFE_DELETE(inputObject);
			throw exception(MESSAGE_NO_ACTIVITY_TITLE.c_str());
		} else {
			inputObject->setTitle(input);
		}
		break;
	}
}

void MariaInterpreter::parseTime(string input, int& hour, int& min) {
	size_t colonPos = input.find(':');
	size_t periodPos = input.find('.');
	vector<string> workingList;

	if (colonPos != string::npos) {
		workingList = tokenizeString(input, ':');

		if (workingList.size() != 2 ||
			!isInteger(workingList[0]) ||
			!isInteger(workingList[1])) {
			throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
		}

		hour = atoi(workingList[0].c_str());
		min = atoi(workingList[1].c_str());

		if (hour < 0 || hour > 23 || min < 0 || min > 59) {
			throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
		}
	} else if (periodPos != string::npos) {
		// 9.30pm
		workingList = tokenizeString(input, '.');

		if (workingList.size() != 2 ||
			!isInteger(workingList[0]) ||
			(workingList[1].size() != 2 &&
			workingList[1].size() != 4)) {
			throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
		}

		hour = atoi(workingList[0].c_str());

		if (isInteger(workingList[1].substr(0, 1))) {
			min = (atoi(workingList[1].substr(0, 1).c_str()) * 10);
		} else {
			throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
		}

		if (isInteger(workingList[1].substr(1, 1))) {
			min += atoi(workingList[1].substr(1, 1).c_str());
		} else {
			throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
		}

		if (workingList[1].size() == 4) {
			if (isStringEqual(workingList[1].substr(2, 2), "am")) {
				if (hour > 12) {
					throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
				}
			} else if (isStringEqual(workingList[1].substr(2, 2), "pm")) {
				if (hour < 12) {
					hour += 12;
				} else {
					throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
				}
			}
		}

		if (hour < 0 || hour > 23 || min < 0 || min > 59) {
			throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
		}
	} else {
		// 9pm
		if (input.size() == 3) {
			if (isInteger(input.substr(0, 1))) {
				hour = atoi(input.substr(0, 1).c_str());
			} else {
				throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
			}

			if (isStringEqual(input.substr(1, 2), "am")) {
				if (hour > 12) {
					throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
				}
			} else if (isStringEqual(input.substr(1, 2), "pm")) {
				if (hour < 12) {
					hour += 12;
				} else {
					throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
				}
			}
			min = 0;
		} else if (input.size() == 4) {
			if (isInteger(input.substr(0, 1))) {
				hour = atoi(input.substr(0, 1).c_str()) * 10;
			} else {
				throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
			}

			if (isInteger(input.substr(1, 1))) {
				hour += atoi(input.substr(1, 1).c_str());
			} else {
				throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
			}

			if (isStringEqual(input.substr(2, 2), "am")) {
				if (hour > 12) {
					throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
				}
			} else if (isStringEqual(input.substr(2, 2), "pm")) {
				if (hour < 12) {
					hour += 12;
				} else {
					throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
				}
			}
			min = 0;
		} else {
			throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
		}
	}
}

MariaTime* MariaInterpreter::parseDateTimeString(vector<string> tokenizedDateTime) {
	int year, month, day, hour, min;
	// Flags to ensure that only the very last of each is captured.
	// Extra dates are ignored.
	bool hasDateString = false;
	bool hasTimeString = false;

	for (int i = tokenizedDateTime.size() - 1; i >= 0; i--) {
		if (hasTime(tokenizedDateTime[i]) && !hasTimeString) {
			try {
				parseTime(tokenizedDateTime[i], hour, min);
			} catch (exception& e) {
				throw;
			}
			hasTimeString = true;
		} else if (isStringEqual(tokenizedDateTime[i], "today") && !hasDateString) {
			year = MariaTime::getCurrentTime().getYear();
			month = MariaTime::getCurrentTime().getMonth();
			day = MariaTime::getCurrentTime().getDay();
			hasDateString = true;
		} else if (isStringEqual(tokenizedDateTime[i], "tomorrow") && !hasDateString) {
			year = MariaTime::getCurrentTime().getYear();
			month = MariaTime::getCurrentTime().getMonth();
			day = MariaTime::getCurrentTime().getDay() + 1;
			hasDateString = true;
		} else if (isStringEqual(tokenizedDateTime[i], "(^(0?[1-9]|[12][0-9]|3[01])[-/](0?[1-9]|1[012])[-/](19|20)?[0-9][0-9]$)") && !hasDateString) {
			int seperatorPos = 0;
			char seperatorArray[2] = { '/', '-' };

			for (int j = 0; j < 2; j++) {
				seperatorPos = tokenizedDateTime[i].find(seperatorArray[j]);

				if (seperatorPos != string::npos) {
					vector<string> workingList = tokenizeString(tokenizedDateTime[i], seperatorArray[j]);

					if (workingList.size() != 3) {
						throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
					}

					day = atoi(workingList[0].c_str());
					month = atoi(workingList[1].c_str());
					year = atoi(workingList[2].c_str());

					if (day <= 0) {
						day = 1;
					} else if (day >= 32) {
						day = 31;
					}

					if (month <= 0) {
						month = 1;
					} else if (month >= 13) {
						month = 12;
					}

					if (year < 100) {
						year += 2000;
					}

					hasDateString = true;
					break;
				}
			}
		} else if (isStringEqual(tokenizedDateTime[i], "jan(uary)?|feb(ruary)?|mar(ch)?|apr(il)?|may|june?|july?|aug(ust)?|sept?(ember)?|oct(tober)?|nov(ember)?|dec(ember)?") && !hasDateString) {
			year = MariaTime::getCurrentTime().getYear();
			month = getMonth(tokenizedDateTime[i]);

			// Check to see if the month is past today's date.
			// If it is, advance by one year.
			if (month < MariaTime::getCurrentTime().getMonth()) {
				year++;
			}

			// Check the previous token if it is a valid day.
			// If it isn't, just set it to 1st of whatever month this is.
			if (i >= 1 && isInteger(tokenizedDateTime[i-1])) {
				day = atoi(tokenizedDateTime[i-1].c_str());

				// Don't know if this check is necessary. Cause of maketime.
				if (day <= 0) {
					day = 1;
				} else if (day >= 32) {
					day = 31;
				}
				i--;
			} else {
				if (month == MariaTime::getCurrentTime().getMonth()) {
					year++;
				}
				day = 1;
			}
			hasDateString = true;
		} else if (isStringEqual(tokenizedDateTime[i], "mon(day)?|tues?(day)?|wed(nesday)?|thur?s?(day)?|fri(day)?|sat(urday)?|(sun)?day") && !hasDateString) {
			year = MariaTime::getCurrentTime().getYear();
			month = MariaTime::getCurrentTime().getMonth();
			day = MariaTime::getCurrentTime().getDay();

			// do that minus magic thingy
			int currentDayOfWeek = MariaTime::getCurrentTime().getDayWeek();
			int inputDayOfWeek = getDayOfWeek(tokenizedDateTime[i]);
			int differenceInDays = inputDayOfWeek - currentDayOfWeek;

			if (differenceInDays < 0) {
				day += (7 - abs(differenceInDays));
			} else {
				day += differenceInDays;
			}

			// Also check if the preceding token is "next".
			// If it is, add a week to this day.
			hasDateString = true;
		}
	}

	if (hasDateString && !hasTimeString) {
		if (year == MariaTime::getCurrentTime().getYear() &&
			month == MariaTime::getCurrentTime().getMonth() &&
			day == MariaTime::getCurrentTime().getDay()) {
				hour = MariaTime::getCurrentTime().getHour()+1;
		} else {
			hour = 9;
		}
		min = 0;
	} else if (!hasDateString && hasTimeString) {
		// Check if the time wanted has already passed. If so, go to next day.
		year = MariaTime::getCurrentTime().getYear();
		month = MariaTime::getCurrentTime().getMonth();
		day = MariaTime::getCurrentTime().getDay();

		if ((hour < MariaTime::getCurrentTime().getHour()) ||
			(hour == MariaTime::getCurrentTime().getHour() && min <= MariaTime::getCurrentTime().getMin())){
			day++;
		}
	} else if (!hasDateString && !hasTimeString) {
		throw exception(MESSAGE_INVALID_DATE_TIME.c_str());
	}

	return new MariaTime(year, month, day, hour, min);
}

bool MariaInterpreter::hasDate(string text) {
	bool isDatePresent = hasDateFormat(text);
	bool isDayOfWeekPresent = hasDayOfWeek(text);
	bool isMonthPresent = hasMonth(text);
	bool isTodayPresent = hasToday(text);
	bool isTomorrowPresent = hasTomorrow(text);

	return (isDatePresent || isDayOfWeekPresent || isMonthPresent || isTodayPresent || isTomorrowPresent);
}

bool MariaInterpreter::hasTime(string text) {
	//TODO: Needs further revision.
	regex timeExpression("([01]?[0-9]|2[0-3])([.:][0-5][0-9])?(\\s*[AaPp][Mm])?", regex_constants::icase);

	return regex_search(text, timeExpression);
}

bool MariaInterpreter::hasDateTime(string text) {
	return (hasDate(text) || hasTime(text));
}

bool MariaInterpreter::hasDateFormat(string text) {
	//TODO: May need further revision.
	regex dateExpression("(by|from|to)[ ](^(0?[1-9]|[12][0-9]|3[01])[-/](0?[1-9]|1[012])[-/](19|20)?[0-9][0-9]$)", regex_constants::icase);

	return regex_search(text, dateExpression);
}

bool MariaInterpreter::hasDayOfWeek(string text) {
	regex dayOfWeekExpression("(by|from|to)[ ](next )?mon(day)?|tues?(day)?|wed(nesday)?|thur?s?(day)?|fri(day)?|sat(urday)?|(sun)?day", regex_constants::icase);

	return regex_search(text, dayOfWeekExpression);
}

bool MariaInterpreter::hasMonth(string text) {
	regex monthExpression("(by|from|to)[ ](0?[1-9]|[12][0-9]|3[01])[ ]jan(uary)?|feb(ruary)?|mar(ch)?|apr(il)?|may|june?|july?|aug(ust)?|sept?(ember)?|oct(tober)?|nov(ember)?|dec(ember)?", regex_constants::icase);

	return regex_search(text, monthExpression);
}

bool MariaInterpreter::hasToday(string text) {
	regex todayExpression("(by|from|to)[ ]today", regex_constants::icase);

	return regex_search(text, todayExpression);
}

bool MariaInterpreter::hasTomorrow(string text) {
	regex tomorrowExpression("(by|from|to)[ ]tomorrow", regex_constants::icase);

	return regex_search(text, tomorrowExpression);
}

string MariaInterpreter::extractFromBackOfString(string text, string delimiter, int& delimiterPos) {
	size_t occ1 = text.find(delimiter);
	size_t occ2;

	if (occ1 != string::npos) {
		do {
			occ2 = text.find(delimiter.c_str(), occ1+1, delimiter.size());

			if (occ2 != string::npos) {
				occ1 = occ2;
			} else {
				delimiterPos = occ1;
				return text.substr(occ1);
			}
		} while (occ2 != string::npos);
	}
	delimiterPos = text.size();
	return "";
}

int MariaInterpreter::getDayOfWeek(string text) {
	regex dayExpression("mon(day)?|tues?(day)?|wed(nesday)?|thur?s?(day)?|fri(day)?|sat(urday)?|(sun)?day", regex_constants::icase);
	smatch matches;
	const string dayExpressionArray[7] = { "mon(day)?", "tues?(day)?", "wed(nesday)?", "thur?s?(day)?", "fri(day)?", "sat(urday)?", "(sun)?day" };

	if (regex_match(text, matches, dayExpression)) {
		for (int i = 0; i < 7; i++) {
			if (isStringEqual(matches[0], dayExpressionArray[i])) {
				return i+1;
			}
		}
	}

	return -1;
}

int MariaInterpreter::getLastDayOfMonth(int year, int month) {
	MariaTime* tempDate = new MariaTime(year, month+1, 0);

	int day = tempDate->getDay();

	SAFE_DELETE(tempDate);

	return day;
}

int MariaInterpreter::getMonth(string text) {
	regex monthExpression("jan(uary)?|feb(ruary)?|mar(ch)?|apr(il)?|may|june?|july?|aug(ust)?|sept?(ember)?|oct(tober)?|nov(ember)?|dec(ember)?", regex_constants::icase);
	smatch matches;
	const string monthExpressionArray[12] = { "jan(uary)?", "feb(ruary)?", "mar(ch)?", "apr(il)?", "may", "june?", "july?", "aug(ust)?", "sept?(ember)?", "oct(tober)?", "nov(ember)?", "dec(ember)?" };

	if (regex_match(text, matches, monthExpression)) {
		for (int i = 0; i < 12; i++) {
			if (isStringEqual(matches[0], monthExpressionArray[i])) {
				return i+1;
			}
		}
	}

	return -1;
}

bool MariaInterpreter::isInteger(string text) {
	return !text.empty() &&
		text.find_first_not_of("0123456789") == string::npos;
}

bool MariaInterpreter::isStringContain(string text, string expr, bool ignoreCasing) {
	regex* expression;

	if (ignoreCasing) {
		expression = new regex(expr, regex_constants::icase);
	} else {
		expression = new regex(expr);
	}

	bool result = regex_search(text, *expression);

	SAFE_DELETE(expression);

	return result;
}

bool MariaInterpreter::isStringEqual(string text, string expr, bool ignoreCasing) {
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

string MariaInterpreter::lowercaseString(string text) {
	string toReturn = "";

	for (int i = 0; i < text.size(); i++ ) {
		toReturn += tolower(text[i]);
	}

	return toReturn;
}

void MariaInterpreter::removeTokens(vector<string> &input, int startPos, int endPos) {
	int lengthToRemove = endPos - startPos;

	for (int i = 0; i < lengthToRemove; i++) {
		input.erase(input.begin()+startPos);
	}
}

string MariaInterpreter::stitchString(vector<string> token, int startPos, int endPos, string delimiter) {
	string toReturn = "";

	for (int i = startPos; i < endPos; i++) {
		toReturn += token[i];

		if (i != endPos - 1) {
			toReturn += delimiter;
		}
	}

	return toReturn;
}

vector<string> MariaInterpreter::tokenizeString(string inputString, char delimiter) {
	vector<string> tokens;
	stringstream ss(inputString);
	string token;

	while (getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

string MariaInterpreter::trimWhiteSpaceLeft(string text) {
	text.erase(text.begin(), find_if(text.begin(), text.end(), not1(ptr_fun<int, int>(isspace))));
	return text;
}

string MariaInterpreter::trimWhiteSpaceRight(string text) {
	text.erase(find_if(text.rbegin(), text.rend(), not1(ptr_fun<int, int>(isspace))).base(), text.end());
	return text;
}

string MariaInterpreter::trimWhiteSpace(string text) {
	return trimWhiteSpaceLeft(trimWhiteSpaceRight(text));
}