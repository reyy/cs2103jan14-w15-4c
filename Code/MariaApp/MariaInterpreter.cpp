#include "MariaInterpreter.h"

MariaInterpreter::MariaInterpreter(map<string, CommandType> *inputCommandList){
	userDefinedCommands = inputCommandList;
}

MariaInterpreter::~MariaInterpreter(void){
}

MariaInterpreter::CommandType MariaInterpreter::getCommandType(string &inputString) {
	if (inputString == "create") {
		inputString = replaceText(inputString, "create", "");
		return MariaInterpreter::AddFloatingTask;
	} else if (inputString == "show") {
		inputString = replaceText(inputString, "show", "");
		return MariaInterpreter::ShowAllTask;
	} else if (inputString == "delete") {
		inputString = replaceText(inputString, "delete", "");
		return MariaInterpreter::DeleteTask;
	} else if (inputString == "exit") {
		inputString = replaceText(inputString, "exit", "");
		return MariaInterpreter::Exit;
	}

	return MariaInterpreter::Invalid;
}
string MariaInterpreter::replaceText(string inputString, string oldText, string newText, bool firstInstanceOnly) {
	regex term(oldText);

	return regex_replace(
		inputString,
		term,
		newText,
		(firstInstanceOnly?regex_constants::format_first_only:regex_constants::match_default));
}