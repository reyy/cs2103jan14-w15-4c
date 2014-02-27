#pragma once
#include<string>
#include<map>
using namespace std;
class MariaInterpreter{

public:
	typedef enum{invalid, addFloating, addDeadline, addTimed, show} CommandType; 
private:
	map<string, CommandType> *userDefinedCommands;

	int apple;
public:
	MariaInterpreter(map<string, CommandType>* inputCommandList = NULL);
	~MariaInterpreter(void);

	CommandType getCommandType(string inputString);
	
	void setApple(int a);
	int getApple();
};

