#include "MariaInterpreter.h"

MariaInterpreter::MariaInterpreter(map<string, CommandType> *inputCommandList){
	userDefinedCommands = inputCommandList;
}

MariaInterpreter::~MariaInterpreter(void){
}

MariaInterpreter::CommandType MariaInterpreter::getCommandType(string &inputString) {
	CommandType command = Invalid;
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

		int fromKeywordPos = inputString.find("from");
		int byKeywordPos = inputString.find("by");

		if (fromKeywordPos == string::npos && byKeywordPos == string::npos) {
			command = AddFloatingTask;
		} else if (fromKeywordPos != string::npos && byKeywordPos == string::npos) {
			command = AddTimedTask;
		} else if (fromKeywordPos == string::npos && byKeywordPos != string::npos) {
			command = AddDeadlineTask;
		}
	} else if (input[0] == "show") {
		inputString = replaceText(inputString, "show", "");
		command = ShowAllTask;
	} else if (input[0] == "edit") {
		inputString = replaceText(inputString, "edit", "");
		command = EditTask;
	} else if (input[0] == "delete") {
		inputString = replaceText(inputString, "delete", "");
		command = DeleteTask;
	} else if (input[0] == "home") {
		inputString = replaceText(inputString, "home", "");
		command = GoToHome;
	} else if (input[0] == "exit") {
		inputString = replaceText(inputString, "exit", "");
		command = Exit;
	} else if (input[0] == "quit") {
		inputString = replaceText(inputString, "quit", "");
		command = Quit;
	}

	inputString = trimWhiteSpace(inputString);

	return command;
}

string MariaInterpreter::getTitle(string &inputString) {
	string title;

	int endOfTitlePos[2];

	endOfTitlePos[0] = inputString.find(" by ");
	endOfTitlePos[1] = inputString.find(" from ");

	if (endOfTitlePos[0] != string::npos) {
		title = inputString.substr(0, endOfTitlePos[0]);
	} else if (endOfTitlePos[1] != string::npos) {
		title = inputString.substr(0, endOfTitlePos[1]);
	} else {
		title = inputString;
	}

	inputString = replaceText(inputString, title, "");
	inputString = trimWhiteSpace(inputString);

	title = trimWhiteSpace(title);

	return title;
}

MariaTime* MariaInterpreter::getStartTime(string &inputString) {
	MariaTime* startTime = NULL;
	int endOfStartStringPos = inputString.find("to");
	string startTimeString;

	if (endOfStartStringPos == string::npos) {
		startTimeString = inputString;
	} else {
		startTimeString = inputString.substr(0, endOfStartStringPos);
	}

	// Format: DD-MM-YY HH:MM
	vector<string> firstPass = tokenizeString(startTimeString);
	vector<string> date = tokenizeString(firstPass[1], '\/');
	vector<string> time = tokenizeString(firstPass[2], ':');

	int day = atoi(date[0].c_str());
	int month = atoi(date[1].c_str());
	// ASSUMPTION: date entered is in 2000s
	int year = atoi(date[2].c_str()) + 2000;

	int hour = atoi(time[0].c_str());
	int min = atoi(time[1].c_str());

	startTime = new MariaTime(year, month, day, hour, min);

	inputString = replaceText(inputString, startTimeString, "");
	inputString = trimWhiteSpace(inputString);

	return startTime;
}

MariaTime* MariaInterpreter::getEndTime(string &inputString) {
	MariaTime* endTime = NULL;

	// Format: DD-MM-YY HH:MM
	vector<string> firstPass = tokenizeString(inputString);
	vector<string> date = tokenizeString(firstPass[1], '\/');
	vector<string> time = tokenizeString(firstPass[2], ':');

	int day = atoi(date[0].c_str());
	int month = atoi(date[1].c_str());
	// ASSUMPTION: date entered is in 2000s
	int year = atoi(date[2].c_str()) + 2000;

	int hour = atoi(time[0].c_str());
	int min = atoi(time[1].c_str());

	endTime = new MariaTime(year, month, day, hour, min);

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
	} else if (input[0] == "exit" && input.size() < 1) {
		return false;
	} else if (input[0] == "quit" && input.size() < 1) {
		return false;
	}

	// For our "create" command, we need to check if it is a floating,
	// deadline or timed task. So we need to see if the respective
	// keywords are present.
	if (input[0] == "create") {
		int keyword[3];

		keyword[0] = inputString.find("from");
		keyword[1] = inputString.find("to");
		keyword[2] = inputString.find("by");

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
