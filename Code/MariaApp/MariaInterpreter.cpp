#include "MariaInterpreter.h"

MariaInterpreter::MariaInterpreter(map<string, CommandType> *inputCommandList){
	userDefinedCommands = inputCommandList;
}

MariaInterpreter::~MariaInterpreter(void){
}

MariaInterpreter::CommandType MariaInterpreter::getCommandType(string &inputString) {
	CommandType command = Invalid;
	vector<string> input;

	if (inputString.size() > 0) {
		input = tokenizeString(inputString);
	} else {
		return command;
	}

	//TODO: Loop through all available keywords using map.

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
	} else if (input[0] == "delete") {
		inputString = replaceText(inputString, "delete", "");
		command = DeleteTask;
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

MariaInterpreter::CommandType MariaInterpreter::getCommandTypeRegex(string &inputString) {
	CommandType command = Invalid;

	regex keywordExpression("create|show|delete|exit");
	smatch result;

	if (!regex_search(inputString, result, keywordExpression)) {
		return CommandType::Invalid;
	}

	if (inputString == "create") {
		inputString = replaceText(inputString, "create", "");
		command = AddFloatingTask;
	} else if (inputString == "show") {
		inputString = replaceText(inputString, "show", "");
		command = ShowAllTask;
	} else if (inputString == "delete") {
		inputString = replaceText(inputString, "delete", "");
		command = DeleteTask;
	} else if (inputString == "exit") {
		inputString = replaceText(inputString, "exit", "");
		command = Exit;
	} else if (inputString == "quit") {
		inputString = replaceText(inputString, "quit", "");
		command = Quit;
	}

	inputString = trimWhiteSpace(inputString);

	return command;
}

string MariaInterpreter::getTitle(string &inputString) {
	string title;

	int endOfTitlePos[2];

	// Current endOfTitlePos keywords:
	// 1. by
	// 2. from
	endOfTitlePos[0] = inputString.find(" by ");
	endOfTitlePos[1] = inputString.find(" from ");

	if (endOfTitlePos[0] != string::npos) {
		title = inputString.substr(0, endOfTitlePos[0]);
		//inputString = replaceText(inputString, " by ", "");
	} else if (endOfTitlePos[1] != string::npos) {
		title = inputString.substr(0, endOfTitlePos[1]);
		//inputString = replaceText(inputString, " from ", "");
	} else {
		title = inputString;
	}
	//TODO: Loop through keyword bank to check for other definitions of endOfTitle keywords.

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

string MariaInterpreter::replaceText(string inputString, string oldText, string newText, bool firstInstanceOnly) {
	regex term(oldText);

	return regex_replace(
		inputString,
		term,
		newText,
		(firstInstanceOnly?regex_constants::format_first_only:regex_constants::match_default));
}

/*vector<string> MariaInterpreter::tokenizeString(string inputString) {
	vector<string> token;
	istringstream iss(inputString);
	copy(istream_iterator<string>(iss),
		istream_iterator<string>(),
		back_inserter<vector<string>>(token));

	return token;
}*/

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
