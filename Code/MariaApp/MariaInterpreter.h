#pragma once

#include <string>
#include <map>
#include <sstream>
#include <functional>
#include <regex>
#include "MariaTime.h"
using namespace std;

class MariaInterpreter{
public:
	typedef enum {
		Invalid,
		AddFloatingTask,
		AddDeadlineTask,
		AddTimedTask,
		EditTask,
		ShowAllTask,
		DeleteTask,
		GoToHome,
		Exit,
		Quit,
	} CommandType;

	MariaInterpreter(map<string, CommandType>* inputCommandList = NULL);
	~MariaInterpreter(void);

	/// Gets a CommandType flag depending on the user's input.
	/// The command keyword would also be removed from the inputString.
	///
	/// @param	inputString	User's input.
	///
	/// @return	The command's flag will be returned on valid input. Invalid flag otherwise.
	CommandType getCommandType(string &inputString);

	/// Gets the task's title from the user's input.
	/// The title would also be removed from the inputString.
	///
	/// @param	inputString	User's input.
	///
	/// @return	The task's title would be returned on valid input. Blank otherwise.
	string getTitle(string &inputString);

	string getNewTitle(string &inputString);

	MariaTime* getStartTime(string &inputString);
	MariaTime* getEndTime(string &inputString);

#ifdef _DEBUG
public:
#else
private:
#endif
	map<string, CommandType> *userDefinedCommands;

	/// Checks the validity of the user's input depending on
	/// the presence of certain keywords, number of words and so on.
	///
	/// @param	User's input.
	///
	/// @return	True if valid. False otherwise.
	bool checkInputValidity(string inputString);

	/// Replaces oldText in the inputString with newText.
	///
	/// @param inputString			String to be modified.
	/// @param oldText				Text to be replaced.
	/// @param newText				Text to replace oldText
	/// @param firstInstanceOnly	Whether only the first instance of oldText will be replaced. Default is true.
	///
	/// @return						String with oldText replaced with newText.
	string replaceText(string inputString, string oldText, string newText, bool firstInstanceOnly = true);

	vector<string> tokenizeString(string inputString, char delimiter = ' ');

	string lowercaseString(string text);

	// To trim whitespace.
	inline string trimWhiteSpaceLeft(string text);
	inline string trimWhiteSpaceRight(string text);
	inline string trimWhiteSpace(string text);
};