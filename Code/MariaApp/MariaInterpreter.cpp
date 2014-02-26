#include "MariaInterpreter.h"


MariaInterpreter::MariaInterpreter(map<string, CommandType> *inputCommandList){
	userDefinedCommands = inputCommandList;
}


MariaInterpreter::~MariaInterpreter(void){
}
