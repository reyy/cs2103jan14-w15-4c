#include <fstream>
#include <sys/stat.h>
#include <vector>
#include "MariaTask.h"

#pragma once
class MariaFileManager
{
public:
	static const string INPUT_FILE;
	static const int ATTRIBUTES_PER_TASK = 7;
	static const string TASK_OPEN_FIELD;
	static const string TASK_TITLE_FIELD;
	static const string TASK_DESC_FIELD;
	static const string TASK_STARTTIME_FIELD;
	static const string TASK_ENDTIME_FIELD;
	static const string TASK_CREATED_FIELD;
	static const string TASK_CLOSE_FIELD;
	static const char NEW_LINE = '\n';
	static const string TIME_FORMAT;

	MariaFileManager(void);
	~MariaFileManager(void);
	vector<MariaTask*>* openFile();
	bool writeFile(vector<MariaTask*>);

private:
	vector<MariaTask*>* readFile(ifstream *fileReader);
	vector<MariaTask*>* createNewFile();
	bool fileExists();

	MariaTask* stringToTask(string inputText[]);
	string taskToString(MariaTask *task);
	MariaTime* stringToTime(string inputText);
	string timeToString(MariaTime* inputTime);

	string getFirstWord(string inputText);
	string removeFirstWord(string inputText, string firstWord);
};

