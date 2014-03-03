#pragma once

#include <vector>
#include <string>
#include "MariaTask.h"
using namespace std;

class MariaTaskManager {
public:
	MariaTaskManager(vector<MariaTask*> *inputTaskList = NULL);
	~MariaTaskManager(void);

	bool addTask(MariaTask);
	bool addTask(std::string name, time_t start = NULL, time_t end = NULL);
	vector<MariaTask*> findTask(std::string searchString);
	vector<MariaTask*> findTask(time_t start, time_t end);

	bool archiveTask(MariaTask);
	bool deleteArchive();

private:
#ifdef _DEBUG
public:
#endif
	vector<MariaTask*> *taskList;
};
