#pragma once
#include<string>
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
	map<string, CommandType> *userDefinedCommands;

public:
	MariaInterpreter(map<string, CommandType>* inputCommandList = NULL);
	~MariaInterpreter(void);

	CommandType getCommandType(string inputString);
};