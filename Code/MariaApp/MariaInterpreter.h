#pragma once
#include<QtCore\qstring.h>
#include<map>
using namespace std;
class MariaInterpreter{

public:
	typedef enum {
		Invalid,
		AddFloatingTask,
		AddDeadlineTask,
		AddTimedTask,
		ShowAllTask,
		DeleteTask,
		Exit,
	} CommandType;

private:
	map<QString, CommandType> *userDefinedCommands;

public:
	MariaInterpreter(map<QString, CommandType>* inputCommandList = NULL);
	~MariaInterpreter(void);

	CommandType getCommandType(QString inputString);
};