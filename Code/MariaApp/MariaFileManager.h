#include <fstream>
#include <sys/stat.h>
#include "MariaTask.h"

#pragma once
class MariaFileManager
{
public:
	static const string INPUT_FILE;
	static const int ATTRIBUTES_PER_TASK = 6;
	static const string TASK_OPEN_FIELD;
	static const string TASK_TITLE_FIELD;
	static const string TASK_DESC_FIELD;
	static const string TASK_STARTTIME_FIELD;
	static const string TASK_ENDTIME_FIELD;
	static const string TASK_CLOSE_FIELD;

	MariaFileManager(void);
	~MariaFileManager(void);
	bool openFile();
	bool writeFile();

private:
	void readFile(ifstream *fileReader);
	bool fileExists();

	MariaTask* stringToTask(string inputText[]);
	string taskToString(MariaTask task);

	string getFirstWord(string inputText);
	string removeFirstWord(string inputText, string firstWord);
};

