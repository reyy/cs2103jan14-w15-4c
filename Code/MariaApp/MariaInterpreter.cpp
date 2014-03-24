#include "MariaInterpreter.h"

MariaInterpreter::MariaInterpreter(map<string, MariaInputObject::COMMAND_TYPE> *inputCommandList) {
	commandKeywordList = inputCommandList;
	if (commandKeywordList == NULL) {
		commandKeywordList = new map<string, MariaInputObject::COMMAND_TYPE>();
	}

	// Create our keyword association to the command.
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
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("up", MariaInputObject::COMMAND_TYPE::GO_UP));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("down", MariaInputObject::COMMAND_TYPE::GO_DOWN));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("exit", MariaInputObject::COMMAND_TYPE::EXIT));
	commandKeywordList->insert(pair<string, MariaInputObject::COMMAND_TYPE>("quit", MariaInputObject::COMMAND_TYPE::EXIT));
}

MariaInterpreter::~MariaInterpreter(void){
	if (commandKeywordList != NULL) {
		delete commandKeywordList;
		commandKeywordList = NULL;
	}
}

MariaInputObject* MariaInterpreter::parseInput(string inputString, MariaStateManager::STATE_TYPE currentState) {
	MariaInputObject* inputObject = new MariaInputObject();

	// Check that we are in the correct state.
	//assert(currentState == MariaStateManager::STATE_TYPE::HOME);

	if (inputString.size() == 0) {
		inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::INVALID);
		// Empty input!
		return inputObject;
	}

	vector<string> tokenizedInput = tokenizeString(inputString);
	map<string, MariaInputObject::COMMAND_TYPE>::iterator commandKeyword;
	vector<MariaTime*> timeList;
	string title = "";

	if (currentState == MariaStateManager::STATE_TYPE::HOME) {
		// Check for command keyword.
		commandKeyword = commandKeywordList->find(lowercaseString(tokenizedInput[0]));

		if (commandKeyword == commandKeywordList->end()) {
			// Command keyword not recognized.
			inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::INVALID);
		} else {
			// Command keyword recognized! Set it!
			inputObject->setCommandType(commandKeyword->second);

			//TODO: Remove the need for this
			tokenizedInput.erase(tokenizedInput.begin());

			switch (inputObject->getCommandType()) {
			case MariaInputObject::COMMAND_TYPE::ADD:
				if (tokenizedInput.size() > 0) {
					title = this->getTitle(tokenizedInput);

					if (title.size() == 0) {
						inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::INVALID);
						return inputObject;
					}
					inputObject->setTitle(title);

					timeList = this->parseDateTimeString(tokenizedInput);

					if (timeList.size() == 0) {
						inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::ADD_FLOATING);
					} else if (timeList.size() == 1) {
						inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::ADD_DEADLINE);
						inputObject->setEndTime(timeList[0]);
					} else if (timeList.size() == 2) {
						inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::ADD_TIMED);
						inputObject->setStartTime(timeList[0]);
						inputObject->setEndTime(timeList[1]);
					}
				} else {
					// There isn't any title
					inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::GO_ADD);
				}
				break;
			case MariaInputObject::COMMAND_TYPE::EDIT:
				if (tokenizedInput.size() > 0) {
					inputObject->setTitle(this->getTitle(tokenizedInput));
					inputObject->setCommandType(this->getEditType(tokenizedInput));

					if (isDate(tokenizedInput[0]) || isTime(tokenizedInput[0])) {
						inputObject->setEditTime(parseDateTimeString(tokenizedInput)[0]);
					} else {
						inputObject->setEditField(this->getEditField(tokenizedInput));
					}
				} else {
					inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::GO_EDIT);
				}
				break;
			case MariaInputObject::COMMAND_TYPE::SHOW:
				if (tokenizedInput.size() == 0) {
					tokenizedInput.push_back("today");
					timeList = this->parseDateTimeString(tokenizedInput);
					inputObject->setEndTime(timeList[0]);
				} else if (tokenizedInput.size() == 1 && isStringMatch(tokenizedInput[0], "all")) {
					inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_ALL);
				} else if (tokenizedInput.size() == 1 && (isStringToday(tokenizedInput[0]) || isStringTomorrow(tokenizedInput[0]))) {
					inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_DATE);
					timeList = this->parseDateTimeString(tokenizedInput);
				} else {
					timeList = this->parseDateTimeString(tokenizedInput);

					if (timeList.size() <= 1) {
						inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_DATE);
						inputObject->setEndTime(timeList[0]);
					} else if (timeList.size() == 2) {
						inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::SHOW_DATE_RANGE);
						inputObject->setStartTime(timeList[0]);
						inputObject->setEndTime(timeList[1]);
					}
				}
				break;
			case MariaInputObject::COMMAND_TYPE::MARK_DONE:
			case MariaInputObject::COMMAND_TYPE::MARK_UNDONE:
				inputObject->setTitle(this->getTitle(tokenizedInput));
				break;
			case MariaInputObject::COMMAND_TYPE::DELETE_TASK:
				if (tokenizedInput.size() > 0) {
					if (isStringMatch(tokenizedInput[0], "all")) {
						inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::DELETE_ALL);
					} else {
						inputObject->setTitle(this->getTitle(tokenizedInput));
					}
				} else {
					inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::GO_DELETE);
				}
				break;
			default:
				break;
			}
		}
	} else if (currentState == MariaStateManager::STATE_TYPE::CONFLICT) {
		// Check for command keyword.
		commandKeyword = commandKeywordList->find(tokenizedInput[0]);

		if (commandKeyword == commandKeywordList->end()) {
			// Command keyword not recognized.
			// Don't worry. Check instead if it is a number.
			if (isInteger(tokenizedInput[0])) {
				inputObject->setOptionID(atoi(tokenizedInput[0].c_str()));
			} else {
				// Nope. NaN LOL.
				inputObject->setCommandType(MariaInputObject::COMMAND_TYPE::INVALID);
				if (tokenizedInput.size() > 0) {
				}
			}
		} else {
			// Command keyword recognized.
			inputObject->setCommandType(commandKeyword->second);

			tokenizedInput.erase(tokenizedInput.begin());

			// Check if the first word after the command keyword is an integer.
			if (isInteger(tokenizedInput[0])) {
				inputObject->setOptionID(atoi(tokenizedInput[0].c_str()));
			} else {
				// Nope. But it may be the title.
				if (tokenizedInput.size() > 0) {
					inputObject->setTitle(this->getTitle(tokenizedInput));
				}
			}
		}
	}

	return inputObject;
}

string MariaInterpreter::getTitle(vector<string> &tokenizedInput) {
	string title = "";
	for (int i = 0; i < tokenizedInput.size(); i++) {
		// Search for the following 'stop' keywords: by, from, to, today, tomorrow, time, date, change
		if (isStringMatch(tokenizedInput[i], "by|from|to")) {
			// Check the next token to see if it is a date or time.
			if (((i + 1) < tokenizedInput.size()) &&
				(isDate(tokenizedInput[i+1]) || isTime(tokenizedInput[i+1]))) {
				// Stitch the tokens up till now to be the title.
				title = stitchString(tokenizedInput, 0, i);
				removeTokens(tokenizedInput, 0, i);
				return title;
			}
		} else if (isDate(tokenizedInput[i]) || isTime(tokenizedInput[i])) {
			// The current token is a date/time.
			// Stitch the tokens up till now to be the title.
			title = stitchString(tokenizedInput, 0, i);
			removeTokens(tokenizedInput, 0, i);
			return title;
		} else if (isStringMatch(tokenizedInput[i], "change") &&
			i < tokenizedInput.size() - 1 &&
			isStringMatch(tokenizedInput[i+1], "title|start|end")) {
			title = stitchString(tokenizedInput, 0, i);
			removeTokens(tokenizedInput, 0, i);
			return title;
		}
	}
	title = stitchString(tokenizedInput, 0, tokenizedInput.size());
	removeTokens(tokenizedInput, 0, tokenizedInput.size());
	return title;
}

MariaInputObject::COMMAND_TYPE MariaInterpreter::getAddType(vector<MariaTime*> &timeList) {
	return MariaInputObject::COMMAND_TYPE::ADD;
}

MariaInputObject::COMMAND_TYPE MariaInterpreter::getEditType(vector<string> &tokenizedInput) {
	if (isStringMatch(tokenizedInput[1], "title")) {
		removeTokens(tokenizedInput, 0, 2);
		return MariaInputObject::COMMAND_TYPE::EDIT_TITLE;
	} else if (isStringMatch(tokenizedInput[1], "start")) {
		removeTokens(tokenizedInput, 0, 2);
		return MariaInputObject::COMMAND_TYPE::EDIT_START_TIME;
	} else if (isStringMatch(tokenizedInput[1], "end")) {
		removeTokens(tokenizedInput, 0, 2);
		return MariaInputObject::COMMAND_TYPE::EDIT_END_TIME;
	}

	return MariaInputObject::COMMAND_TYPE::EDIT;
}

string MariaInterpreter::getEditField(vector<string> &tokenizedInput) {
	return stitchString(tokenizedInput, 0, tokenizedInput.size());
}

vector<MariaTime*> MariaInterpreter::parseDateTimeString(vector<string> &tokenizedInput) {
	vector<MariaTime*> timeObjectList;
	vector<string> workingList;

	for (int i = 0; i < tokenizedInput.size(); i++) {
		if (isDate(tokenizedInput[i])) {
			// Check the next token to see if it is a time string
			if ((i < tokenizedInput.size() - 1) &&
				isTime(tokenizedInput[i+1])) {
				// Case: DT??
				// Extract both the date and time from the tokens.
				// Insert them into a new mariatime object.
				workingList.push_back(tokenizedInput[i]);
				workingList.push_back(tokenizedInput[i+1]);
				timeObjectList.push_back(parseDateTime(workingList, true, true));
				workingList.clear();
				// Since this is a complete mariatime object, we can skip one object.
				i++;
			} else {
				// Case: D??? & Case: D/D?
				workingList.push_back(tokenizedInput[i]);
				timeObjectList.push_back(parseDateTime(workingList, true, false));
				workingList.clear();
			}
		} else if (isTime(tokenizedInput[i])) {
			workingList.push_back(tokenizedInput[i]);
			timeObjectList.push_back(parseDateTime(workingList, false, true));
			workingList.clear();
		}
	}
	return timeObjectList;
}

MariaTime* MariaInterpreter::parseDateTime(vector<string> dateTimeList, bool hasDate, bool hasTime) {
	assert(dateTimeList.size() > 0 && dateTimeList.size() <= 2);

	// Here must be: date OR time OR date/time

	MariaTime* toReturn = NULL;
	vector<string> workingList;
	int delimiterPos;
	int year, month, day, hour, min;

	if (dateTimeList.size() == 1) {
		if (hasDate) {
			parseDate(dateTimeList[0], year, month, day);

			toReturn = new MariaTime(MariaTime::getCurrentTime());
			toReturn->setYear(year);
			toReturn->setMonth(month);
			toReturn->setDay(day);
			toReturn->setHour(23);
			toReturn->setMin(59);
		} else if (hasTime) {
			parseTime(dateTimeList[0], hour, min);

			toReturn = new MariaTime(MariaTime::getCurrentTime());
			toReturn->setHour(hour);
			toReturn->setMin(min);
		}
	} else {
		// Just construct the mariatime object
		if (isStringToday(dateTimeList[0])) {
			toReturn = new MariaTime(MariaTime::getCurrentTime());

			parseTime(dateTimeList[1], hour, min);
			toReturn->setHour(hour);
			toReturn->setMin(min);
		} else if (isStringTomorrow(dateTimeList[0])) {
			toReturn = new MariaTime(MariaTime::getCurrentTime());
			toReturn->setDay(toReturn->getDay()+1);

			parseTime(dateTimeList[1], hour, min);
			toReturn->setHour(23);
			toReturn->setMin(59);

			MariaTime* temp = toReturn;
			toReturn = new MariaTime(*temp);
			delete temp;
			temp = NULL;
		} else {
			parseDate(dateTimeList[0], year, month, day);
			parseTime(dateTimeList[1], hour, min);

			toReturn = new MariaTime();
			toReturn->setYear(year);
			toReturn->setMonth(month);
			toReturn->setDay(day);
			toReturn->setHour(hour);
			toReturn->setMin(min);
		}
	}

	toReturn->setSec(0);
	return toReturn;
}

void MariaInterpreter::parseDate(string text, int &year, int &month, int &day) {
	vector<string> workingList;
	int delimiterPos;
	const char dateDelimiterArray[3] = { '/', '-', '.' };

	if (isStringToday(text)) {
		year = MariaTime::getCurrentTime().getYear();
		month = MariaTime::getCurrentTime().getMonth();
		day = MariaTime::getCurrentTime().getDay();
	} else if (isStringTomorrow(text)) {
		year = MariaTime::getCurrentTime().getYear();
		month = MariaTime::getCurrentTime().getMonth();
		day = MariaTime::getCurrentTime().getDay()+1;
	}

	// It's a date object only.
	for (int i = 0; i < 3; i++) {
		if (text.find(dateDelimiterArray[i]) != string::npos) {
			// Process.
			workingList = tokenizeString(text, dateDelimiterArray[i]);

			assert(workingList.size() == 3);
			year = atoi(workingList[2].c_str());
			month = atoi(workingList[1].c_str());
			day = atoi(workingList[0].c_str());

			year < 100 ? year += 2000 : 0;

			workingList.clear();

			return;
		}
	}
}

void MariaInterpreter::parseTime(string text, int &hour, int &min) {
	// It's a time object only.
	bool hasDelimiter = false;
	const char timeDelimiterArray[2] = { ':', '.' };
	int delimiterPos;
	vector<string> workingList;

	for (int i = 0; i < 2; i++) {
		if (text.find(timeDelimiterArray[i]) != string::npos) {
			// Process.
			workingList = tokenizeString(text, timeDelimiterArray[i]);

			assert(workingList.size() == 2);
			hour = atoi(workingList[0].c_str());
			min = atoi(workingList[1].c_str());

			hasDelimiter = true;

			return;
		}
	}

	if (!hasDelimiter) {
		// Check the length of the string.
		// Process it manually.
		string temp;

		if (doesStringContain(text, "am|pm|mn|nn")) {
			// We need to process this differently.
		} else {
			if (text.size() == 3) {
				// E.g.: 111 - 1:11
				temp = text[0];
				hour = atoi(temp.c_str());
				temp = text.substr(1, 2);
				min = atoi(temp.c_str());
			} else if (text.size() == 4) {
				// E.g.: 2359
				temp = text.substr(0, 2);
				hour = atoi(temp.c_str());
				temp = text.substr(2, 2);
				min = atoi(temp.c_str());
			}
		}
	}
}

string MariaInterpreter::replaceText(string inputString, string oldText, string newText, bool firstInstanceOnly) {
	regex term(oldText);

	return regex_replace(
		inputString, 
		term, 
		newText, 
		(firstInstanceOnly?regex_constants::format_first_only:regex_constants::match_default));
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

string MariaInterpreter::lowercaseString(string text) {
	string toReturn = "";

	for (int i = 0; i < text.size(); i++ ) {
		toReturn += tolower(text[i]);
	}

	return toReturn;
}

bool MariaInterpreter::isInteger(string text) {
	return !text.empty() &&
		text.find_first_not_of("0123456789") == string::npos;
}

bool MariaInterpreter::isDate(string text) {
	text = lowercaseString(text);

	//TODO: Check "24 june"

	return isStringToday(text) ||
		isStringTomorrow(text) ||
		isStringDayFormat(text) ||
		isStringDateFormat(text);
}

bool MariaInterpreter::isStringToday(string text) {
	regex todayExpression("today");

	return regex_match(text, todayExpression);
}

bool MariaInterpreter::isStringTomorrow(string text) {
	regex tomorrowExpression("tomorrow");

	return regex_match(text, tomorrowExpression);
}

bool MariaInterpreter::isStringDayFormat(string text) {
	regex dayExpression("mon(day)?|tues?(day)?|wed(nesday)?|thur?s?(day)?|fri(day)?|sat(urday)?|sunday");

	return regex_match(text, dayExpression);
}

bool MariaInterpreter::isStringDateFormat(string text) {
	regex dateExpression("[0123][0-9](/|.|-)[01][0-9](/|.|-)[0-9][0-9]([0-9][0-9])?");

	return regex_match(text, dateExpression);
}

bool MariaInterpreter::isTime(string text) {
	regex timeExpression("([012]?[0-9](:|.)?[0-5][0-9])|([01]?[0-9](:|.)?[0-5][0-9])(am|pm)");
	if (regex_match(text, timeExpression)) {
		//TODO: Check is valid time.
		return true;
	}

	return false;
}

int MariaInterpreter::getDayOfWeek(string text) {
	regex dayExpression("mon(day)?|tues?(day)?|wed(nesday)?|thur?s?(day)?|fri(day)?|sat(urday)?|sunday");
	smatch matches;
	const string dayExpressionArray[7] = { "mon(day)?", "tues?(day)?", "wed(nesday)?", "thur?s?(day)?", "fri(day)?", "sat(urday)?", "sunday"};

	if (regex_match(text, matches, dayExpression)) {
		for (int i = 0; i < 7; i++) {
			if (isStringMatch(matches[0], dayExpressionArray[i])) {
				return i;
			}
		}
	}

	return -1;
}

bool MariaInterpreter::isStringMatch(string text, string expr, bool ignoreCasing) {
	regex* expression;

	if (ignoreCasing) {
		expression = new regex(expr, regex_constants::icase);
	} else {
		expression = new regex(expr);
	}

	bool result = regex_match(text, *expression);

	delete expression;
	expression = NULL;

	return result;
}

bool MariaInterpreter::doesStringContain(string text, string expr, bool ignoreCasing) {
	regex* expression;

	if (ignoreCasing) {
		expression = new regex(expr, regex_constants::icase);
	} else {
		expression = new regex(expr);
	}

	bool result = regex_search(text, *expression);

	delete expression;
	expression = NULL;

	return result;
}

void MariaInterpreter::getDate(string input, int &day, int &month, int &year) {
}

void MariaInterpreter::getTime(string input, int &hour, int &min) {
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

void MariaInterpreter::removeTokens(vector<string> &input, int startPos, int endPos) {
	int lengthToRemove = endPos - startPos;

	for (int i = 0; i < lengthToRemove; i++) {
		input.erase(input.begin()+startPos);
	}
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
