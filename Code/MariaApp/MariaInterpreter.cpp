#include "MariaInterpreter.h"

MariaInterpreter::MariaInterpreter(map<string, MariaInputObject::CommandType> *inputCommandList){
	userDefinedCommands = inputCommandList;
}

MariaInterpreter::~MariaInterpreter(void){
}

MariaInputObject* MariaInterpreter::parseInput(string inputString) {
	MariaInputObject* inputObject = new MariaInputObject();

	inputObject->setOriginalInput(inputString);
	inputObject->setValidity(checkInputValidity(inputString));
	inputObject->isValid() ? inputObject->setCommandType(getCommandType(inputString)) : inputObject->setCommandType(MariaInputObject::CommandInvalid);

	switch (inputObject->getCommandType()) {
	case MariaInputObject::CommandAdd:
		inputObject->setTitle(this->getTitle(inputString));
		inputObject->setAddType(this->getAddType(inputString));
		if (inputObject->getAddType() == MariaInputObject::AddTimed) {
			inputObject->setStartTime(this->getStartTime(inputString));
			inputObject->setEndTime(this->getEndTime(inputString));
		} else if (inputObject->getAddType() == MariaInputObject::AddDeadline) {
			inputObject->setEndTime(this->getEndTime(inputString));
		}
		break;
	case MariaInputObject::CommandEdit:
		inputObject->setTitle(this->getTitle(inputString));
		inputObject->setEditField(this->getEditField(inputString));
		break;
	case MariaInputObject::CommandDelete:
		inputObject->setTitle(this->getTitle(inputString));
		break;
	default:
		break;
	}

	return inputObject;
}

MariaInputObject::CommandType MariaInterpreter::getCommandType(string &inputString) {
	MariaInputObject::CommandType command = MariaInputObject::CommandInvalid;
	vector<string> input;

	if (!checkInputValidity(inputString)) {
		return command;
	}

	if (inputString.size() > 0) {
		inputString = trimWhiteSpace(inputString);
		input = tokenizeString(inputString);
	} else {
		return command;
	}

	if (input[0] == "create") {
		inputString = replaceText(inputString, "create", "");
		command = MariaInputObject::CommandAdd;
	} else if (input[0] == "show") {
		inputString = replaceText(inputString, "show", "");
		command = MariaInputObject::CommandShowAll;
	} else if (input[0] == "edit") {
		inputString = replaceText(inputString, "edit", "");
		command = MariaInputObject::CommandEdit;
	} else if (input[0] == "delete") {
		inputString = replaceText(inputString, "delete", "");
		command = MariaInputObject::CommandDelete;
	} else if (input[0] == "home") {
		inputString = replaceText(inputString, "home", "");
		command = MariaInputObject::CommandGoToHome;
	} else if (input[0] == "exit") {
		inputString = replaceText(inputString, "exit", "");
		command = MariaInputObject::CommandExit;
	}

	inputString = trimWhiteSpace(inputString);

	return command;
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

string MariaInterpreter::getTitle(string &inputString) {
	string title;

	int endOfTitlePos[3];

	endOfTitlePos[0] = inputString.find(" by ");
	endOfTitlePos[1] = inputString.find(" from ");
	endOfTitlePos[2] = inputString.find(" change ");

	if (endOfTitlePos[0] != string::npos) {
		title = inputString.substr(0, endOfTitlePos[0]);
	} else if (endOfTitlePos[1] != string::npos) {
		title = inputString.substr(0, endOfTitlePos[1]);
	} else if (endOfTitlePos[2] != string::npos) {
		title = inputString.substr(0, endOfTitlePos[2]);
	} else {
		title = inputString;
	}

	inputString = replaceText(inputString, title, "");
	inputString = trimWhiteSpace(inputString);

	title = trimWhiteSpace(title);

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
		vector<string> date = tokenizeString(firstPass[1], '\/');

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
		vector<string> date = tokenizeString(firstPass[1], '\/');

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

bool MariaInterpreter::checkInputValidity(string inputString) {
	if (inputString.size() <= 0) {
		return false;
	}

	// Let's get rid of any unnecessary spaces in the inputString.
	inputString = trimWhiteSpace(inputString);

	// Tokenize the inputString to help in the validation of the input.
	vector<string> input = tokenizeString(inputString);

	// Time to check the first word, which is supposed to be a keyword.
	input[0] = lowercaseString(input[0]);
	if (input[0] != "create" &&
		input[0] != "edit" &&
		input[0] != "show" &&
		input[0] != "delete" &&
		input[0] != "home" &&
		input[0] != "exit" &&
		input[0] != "quit") {
		return false;
	}

	// Now let's check if the input fulfils the minimum number of tokens.
	if (input[0] == "create" && input.size() < 2) {
		return false;
	} else if (input[0] == "edit" && input.size() < 2) {
		return false;
	} else if (input[0] == "show" && input.size() < 1) {
		return false;
	} else if (input[0] == "delete" && input.size() < 2) {
		return false;
	} else if (input[0] == "home" && input.size() < 1) {
		return false;
	} else if ((input[0] == "exit" || input[0] == "quit") && input.size() < 1) {
		return false;
	}

	// For our "create" command, we need to check if it is a floating,
	// deadline or timed task. So we need to see if the respective
	// keywords are present.
	if (input[0] == "create") {
		int keyword[3];

		keyword[0] = inputString.find(" from ");
		keyword[1] = inputString.find(" to ");
		keyword[2] = inputString.find(" by ");

		if (keyword[0] == string::npos && keyword[1] == string::npos && keyword[2] == string::npos) {
			// Floating task.
		} else if (keyword[0] == string::npos && keyword[1] == string::npos && keyword[2] != string::npos) {
			// Deadline task.
		} else if (keyword[0] != string::npos && keyword[1] != string::npos && keyword[2] == string::npos) {
			// Timed task.
		} else {
			// Invalid format.
			return false;
		}
	}

	if (input[0] == "edit") {
		int keyword[2];

		keyword[0] = inputString.find(" change ");
		keyword[1] = inputString.find( "title ");

		if (keyword[0] == string::npos || keyword[1] == string::npos) {
			return false;
		}
	}

	// Seems that all the requirements check out! Our input is valid!
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
