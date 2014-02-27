#include "MariaInterpreter.h"

MariaInterpreter::MariaInterpreter(map<string, CommandType> *inputCommandList){
	userDefinedCommands = inputCommandList;
}

MariaInterpreter::~MariaInterpreter(void){
}

MariaInterpreter::CommandType MariaInterpreter::getCommandType(string inputString) {
	if (inputString == "create") {
		return MariaInterpreter::AddFloatingTask;
	} else if (inputString == "show") {
		return MariaInterpreter::ShowAllTask;
	} else if (inputString == "delete") {
		return MariaInterpreter::DeleteTask;
	} else if (inputString == "exit") {
		return MariaInterpreter::Exit;
	}

	return MariaInterpreter::Invalid;
}