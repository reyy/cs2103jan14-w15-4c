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
		command = AddFloatingTask;
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

	endOfTitlePos[0] = inputString.find(" by ");
	endOfTitlePos[1] = inputString.find(" from ");

	if (endOfTitlePos[0] != string::npos) {
		title = inputString.substr(0, endOfTitlePos[0]);
		inputString = replaceText(inputString, " by ", "");
	} else if (endOfTitlePos[1] != string::npos) {
		title = inputString.substr(0, endOfTitlePos[1]);
		inputString = replaceText(inputString, " from ", "");
	} else {
		title = inputString;
		inputString = replaceText(inputString, title, "");
	}
	//TODO: Loop through keyword bank to check for other definitions of endOfTitle keywords.

	inputString = replaceText(inputString, title, "");
	inputString = trimWhiteSpace(inputString);

	title = trimWhiteSpace(title);

	return title;
}

string MariaInterpreter::replaceText(string inputString, string oldText, string newText, bool firstInstanceOnly) {
	regex term(oldText);

	return regex_replace(
		inputString,
		term,
		newText,
		(firstInstanceOnly?regex_constants::format_first_only:regex_constants::match_default));
}

vector<string> MariaInterpreter::tokenizeString(string inputString) {
	vector<string> token;
	istringstream iss(inputString);
	copy(istream_iterator<string>(iss),
		istream_iterator<string>(),
		back_inserter<vector<string>>(token));

	return token;
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