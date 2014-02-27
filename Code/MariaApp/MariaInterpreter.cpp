#include "MariaInterpreter.h"


MariaInterpreter::MariaInterpreter(map<string, CommandType> *inputCommandList){
	userDefinedCommands = inputCommandList;
}


MariaInterpreter::~MariaInterpreter(void){
}

void MariaInterpreter::setApple(int a){
	apple = a;
}

int MariaInterpreter::getApple() {
	return apple;
}