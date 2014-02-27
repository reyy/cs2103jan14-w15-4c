#pragma once

#include <string>
#include <map>
#include <regex>
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

	MariaInterpreter(map<string, CommandType>* inputCommandList = NULL);
	~MariaInterpreter(void);

	// Returns a CommandType depending on the first word in the
	// inputString passed in. Upon reading a valid Command keyword,
	// that keyword is removed from the inputString itself.
	//
	// If no valid keyword is detected, a Invalid CommandType is
	// returned.
	CommandType getCommandType(string &inputString);

private:
	map<string, CommandType> *userDefinedCommands;

	// Returns a new string that has the oldText replaced with
	// the newText from the inputString itself. Also, firstInstanceOnly
	// determines if only the first instance of the oldText
	// is replaced.
	//
	// By default, firstInstanceOnly is set to true.
	string replaceText(string inputString, string oldText, string newText, bool firstInstanceOnly = true);
};