#include "MariaInterpreter.h"

MariaInterpreter::MariaInterpreter(map<QString, CommandType> *inputCommandList){
	userDefinedCommands = inputCommandList;
}

MariaInterpreter::~MariaInterpreter(void){
}

MariaInterpreter::CommandType MariaInterpreter::getCommandType(QString &inputString) {

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
QString MariaInterpreter::replaceText(QString inputString, string oldText, string newText, bool firstInstanceOnly) {
	regex term(oldText);

	return QString::fromStdString(regex_replace(
		inputString.toStdString(),
		term,
		newText,
		(firstInstanceOnly?regex_constants::format_first_only:regex_constants::match_default)));
}