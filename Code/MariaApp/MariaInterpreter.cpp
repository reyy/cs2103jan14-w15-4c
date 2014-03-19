#include "MariaInterpreter.h"

MariaInterpreter::MariaInterpreter(map<string, MariaInputObject::CommandType> *inputCommandList){
	commandKeywordList = inputCommandList;
	if (commandKeywordList == NULL) {
		commandKeywordList = new map<string, MariaInputObject::CommandType>();
	}

	// Create our keyword association to the command.
	//TODO: Put this into an ini file.
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("add", MariaInputObject::CommandAdd));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("create", MariaInputObject::CommandAdd));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("edit", MariaInputObject::CommandEdit));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("update", MariaInputObject::CommandEdit));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("show", MariaInputObject::CommandShowAll));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("view", MariaInputObject::CommandShowAll));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("delete", MariaInputObject::CommandDelete));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("remove", MariaInputObject::CommandDelete));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("clear", MariaInputObject::CommandDeleteAll));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("home", MariaInputObject::CommandGoToHome));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("up", MariaInputObject::CommandGoUp));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("down", MariaInputObject::CommandGoDown));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("exit", MariaInputObject::CommandExit));
	commandKeywordList->insert(pair<string, MariaInputObject::CommandType>("quit", MariaInputObject::CommandExit));
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

	if (inputString.size() < 0) {
		inputObject->setValidity(false);
		// Empty input!
		return inputObject;
	}

	vector<string> tokenizedInput = tokenizeString(inputString);
	map<string, MariaInputObject::CommandType>::iterator commandKeyword;

	if (currentState == MariaStateManager::STATE_TYPE::HOME) {
		// Check for command keyword.
		commandKeyword = commandKeywordList->find(tokenizedInput[0]);

		if (commandKeyword == commandKeywordList->end()) {
			// Command keyword not recognized.
			inputObject->setValidity(false);
		} else {
			// Command keyword recognized! Set it!
			inputObject->setCommandType(commandKeyword->second);

			//TODO: Remove the need for this
			tokenizedInput.erase(tokenizedInput.begin());

			// Time to get the title.
			inputObject->setTitle(this->getTitle(tokenizedInput));

			switch (inputObject->getCommandType()) {
			case MariaInputObject::CommandAdd:
				//this->parseTime(tokenizedInput);
				//_logger.push_back("Log: parseInput(): CommandAdd");
				inputObject->setAddType(this->getAddType(inputString));
				if (inputObject->getAddType() == MariaInputObject::AddTimed) {
					inputObject->setStartTime(this->getStartTime(inputString));
					inputObject->setEndTime(this->getEndTime(inputString));
				} else if (inputObject->getAddType() == MariaInputObject::AddDeadline) {
					inputObject->setEndTime(this->getEndTime(inputString));
				}
				break;
			case MariaInputObject::CommandEdit:
				//_logger.push_back("Log: parseInput(): CommandEdit");
				inputObject->setEditField(this->getEditField(inputString));
				break;
			case MariaInputObject::CommandDelete:
				//_logger.push_back("Log: parseInput(): CommandDelete");
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
				inputObject->setValidity(false);
			}
		} else {
			// Command keyword recognized.
			inputObject->setCommandType(commandKeyword->second);

			// Check if the first word after the command keyword is an integer.
			if (isInteger(tokenizedInput[1])) {
				inputObject->setOptionID(atoi(tokenizedInput[1].c_str()));
			} else {
				// Nope. But it may be the title.
				//inputObject->setTitle(this->getTitle(inputString));
			}
		}
	}

	inputObject->setValidity(true);
	return inputObject;
}

MariaInputObject::AddType MariaInterpreter::getAddType(string &inputString) {
	MariaInputObject::AddType addType = MariaInputObject::AddNone;

	int fromKeywordPos = inputString.find("from ");
	int byKeywordPos = inputString.find("by ");

	if (fromKeywordPos == string::npos && byKeywordPos == string::npos) {
		addType = MariaInputObject::AddFloating;
	} else if (fromKeywordPos != string::npos && byKeywordPos == string::npos) {
		addType = MariaInputObject::AddTimed;
	} else if (fromKeywordPos == string::npos && byKeywordPos != string::npos) {
		addType = MariaInputObject::AddDeadline;
	}
	return addType;
}

string MariaInterpreter::getTitle(vector<string> &tokenizedInput) {
	string title = "";
	for (int i = 0; i < tokenizedInput.size(); i++) {
		// Search for the following 'stop' keywords: by, from, to, today, tomorrow, time
		if (tokenizedInput[i] == "by" ||
			tokenizedInput[i] == "from" ||
			tokenizedInput[i] == "to") {
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
		}
	}
	title = stitchString(tokenizedInput, 0, tokenizedInput.size());
	removeTokens(tokenizedInput, 0, tokenizedInput.size());
	return title;
}

string MariaInterpreter::getEditField(string &inputString) {
	return inputString.substr(inputString.find("title")+6, inputString.size());
}

MariaTime* MariaInterpreter::getStartTime(string &inputString) {
	MariaTime* startTime = NULL;
	int endOfStartStringPos = inputString.find(" to ");
	string startTimeString;

	if (endOfStartStringPos == string::npos) {
		startTimeString = inputString;
	} else {
		startTimeString = inputString.substr(0, endOfStartStringPos);
	}

	// Format: DD-MM-YY HH:MM
	vector<string> firstPass = tokenizeString(startTimeString);
	vector<string> date;
	vector<string> time;
	int year, month, day, hour, min;

	if (firstPass[1] == "today") {
		startTime = new MariaTime(MariaTime::getCurrentTime());
		time = tokenizeString(firstPass[2], ':');
		hour = atoi(time[0].c_str());
		min = atoi(time[1].c_str());
		startTime->setHour(hour);
		startTime->setMin(min);
	} else if (firstPass[1] == "tomorrow") {
		startTime = new MariaTime(MariaTime::getCurrentTime());
		startTime->setDay(startTime->getDay()+1);
		time = tokenizeString(firstPass[2], ':');
		hour = atoi(time[0].c_str());
		min = atoi(time[1].c_str());
		startTime->setHour(hour);
		startTime->setMin(min);
	} else {
		vector<string> date = tokenizeString(firstPass[1], 47);

		day = atoi(date[0].c_str());
		month = atoi(date[1].c_str());
		// ASSUMPTION: date entered is in 2000s
		year = atoi(date[2].c_str()) + 2000;

		time = tokenizeString(firstPass[2], ':');
		hour = atoi(time[0].c_str());
		min = atoi(time[1].c_str());

		startTime = new MariaTime(year, month, day, hour, min);
	}

	inputString = replaceText(inputString, startTimeString, "");
	inputString = trimWhiteSpace(inputString);

	return startTime;
}

MariaTime* MariaInterpreter::getEndTime(string &inputString) {
	MariaTime* endTime = NULL;

	// Format: DD-MM-YY HH:MM
	vector<string> firstPass = tokenizeString(inputString);
	vector<string> date;
	vector<string> time;
	int year, month, day, hour, min;

	if (firstPass[1] == "today") {
		endTime = new MariaTime(MariaTime::getCurrentTime());
		time = tokenizeString(firstPass[2], ':');
		hour = atoi(time[0].c_str());
		min = atoi(time[1].c_str());
		endTime->setHour(hour);
		endTime->setMin(min);
	} else if (firstPass[1] == "tomorrow") {
		endTime = new MariaTime(MariaTime::getCurrentTime());
		endTime->setDay(endTime->getDay()+1);
		time = tokenizeString(firstPass[2], ':');
		hour = atoi(time[0].c_str());
		min = atoi(time[1].c_str());
		endTime->setHour(hour);
		endTime->setMin(min);
	} else {
		vector<string> date = tokenizeString(firstPass[1], 47);

		day = atoi(date[0].c_str());
		month = atoi(date[1].c_str());
		// ASSUMPTION: date entered is in 2000s
		year = atoi(date[2].c_str()) + 2000;

		time = tokenizeString(firstPass[2], ':');
		hour = atoi(time[0].c_str());
		min = atoi(time[1].c_str());

		endTime = new MariaTime(year, month, day, hour, min);
	}

	inputString = replaceText(inputString, inputString, "");
	inputString = trimWhiteSpace(inputString);

	return endTime;
}

vector<MariaTime*> MariaInterpreter::parseDateTime(vector<string> &tokenizedInput) {
	int dateCount = 0;
	int timeCount = 0;

	for (string token : tokenizedInput) {
		isDate(token)?dateCount++:0;
		isTime(token)?timeCount++:0;
	}

	if (dateCount == 2 || timeCount == 2) {
		// timed task
		int apple = 0;
	} else if ((dateCount == 1 && timeCount <= 1) ||
			(dateCount == 0 && timeCount == 1)) {
		// deadline task
		int banana = 1;
	} else {
		// floating task
		int coconut = 2;
	}
	vector<MariaTime*> toReturn;
	return toReturn;
}

MariaTime* MariaInterpreter::parseDateTime(vector<string> dateTimeList) {
	assert(dateTimeList.size() > 0 && dateTimeList.size() <= 2);

	// Here must be: date OR time OR date/time

	MariaTime* toReturn = NULL;

	if (dateTimeList.size() == 1) {
		if (isDate(dateTimeList[0])) {
			// It's a date! *winks*
			// But first, we need to know what kind of date it is.
			// Use portions of isDate
		} else if (isTime(dateTimeList[0])) {
			toReturn = new MariaTime(MariaTime::getCurrentTime());
			// Need to do comparison, if currentTime is already past set time.
		}
	} else {
		// Need to know what kind of date it is...
		// Also need to know what kind of time it is...
	}
	return toReturn;
}

bool MariaInterpreter::checkInputValidity(string inputString) {
	//_logger.push_back("Log: checkInputValidity(): START");
	if (inputString.size() <= 0) {
		//_logger.push_back("WARNING: Input is empty.");
		return false;
	}

	// Let's get rid of any unnecessary spaces in the inputString.
	inputString = trimWhiteSpace(inputString);

	// Tokenize the inputString to help in the validation of the input.
	vector<string> input = tokenizeString(inputString);

	// Time to check the first word, which is supposed to be a keyword.
	input[0] = lowercaseString(input[0]);
	map<string, MariaInputObject::CommandType>::iterator keyword = commandKeywordList->find(input[0]);
	if (keyword == commandKeywordList->end()) {
		//_logger.push_back("WARNING: Command keyword not recognized.");
		return false;
	}

	// Now let's check if the input fulfils the minimum number of tokens.
	if ((keyword->second == MariaInputObject::CommandAdd && input.size() < 2) ||
		(keyword->second == MariaInputObject::CommandEdit && input.size() < 2) ||
		(keyword->second == MariaInputObject::CommandShowAll && input.size() < 1) ||
		(keyword->second == MariaInputObject::CommandDelete && input.size() < 2) ||
		(keyword->second == MariaInputObject::CommandDeleteAll && input.size() < 1) ||
		(keyword->second == MariaInputObject::CommandGoToHome && input.size() < 1) ||
		(keyword->second == MariaInputObject::CommandExit && input.size() < 1)) {
			//_logger.push_back("WARNING: Input not of sufficient length.");
		return false;
	}

	// For our "create" command, we need to check if it is a floating,
	// deadline or timed task. So we need to see if the respective
	// keywords are present.
	if (input[0] == "create") {
		//_logger.push_back("Log: CommandAdd");
		int keyword[3];

		keyword[0] = inputString.find(" from ");
		keyword[1] = inputString.find(" to ");
		keyword[2] = inputString.find(" by ");

		if (keyword[0] == string::npos && keyword[1] == string::npos && keyword[2] == string::npos) {
			// Floating task.
			//_logger.push_back("Log: CommandAdd: Floating Task request detected.");
		} else if (keyword[0] == string::npos && keyword[1] == string::npos && keyword[2] != string::npos) {
			// Deadline task.
			//_logger.push_back("Log: CommandAdd: Deadline Task request detected.");
		} else if (keyword[0] != string::npos && keyword[1] != string::npos && keyword[2] == string::npos) {
			// Timed task.
			//_logger.push_back("Log: CommandAdd: Timed Task request detected.");
		} else {
			// Invalid format.
			//_logger.push_back("WARNING: Input does not match format for adding a task.");
			return false;
		}
	}

	if (input[0] == "edit") {
		//_logger.push_back("Log: CommandEdit");
		int keyword[2];

		keyword[0] = inputString.find(" change ");
		keyword[1] = inputString.find( "title ");

		if (keyword[0] == string::npos || keyword[1] == string::npos) {
			//_logger.push_back("WARNING: Input does not match format for editing a task.");
			return false;
		}
		//_logger.push_back("Log: CommandEdit: Title change request detected.");
	}

	// Seems that all the requirements check out! Our input is valid!
	//_logger.push_back("Log: checkInputValidity(): END");
	return true;
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

	for (int i = 0; i < text.size(); i++) {
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

	// Check if it is a day format
	regex dayExpression("today|tomorrow|mon(day)?|tues?(day)?|wed(nesday)?|thur?s?(day)?|fri(day)?|sat(urday)?|sunday");
	if (regex_match(text, dayExpression)) {
		return true;
	}

	// Next check if it is a date format
	regex dateExpression("[0123][0-9]/[01][0-9]/[0-9][0-9]([0-9][0-9])?");
	if (regex_match(text, dateExpression)) {
		//TODO: Check is valid date.
		return true;
	}

	//TODO: Check "24 june"

	return false;
}

bool MariaInterpreter::isTime(string text) {
	regex timeExpression("([012]?[0-9]:?[0-5][0-9])|([01]?[0-9]:?[0-5][0-9])(am|pm)");
	if (regex_match(text, timeExpression)) {
		//TODO: Check is valid time.
		return true;
	}

	return false;
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
