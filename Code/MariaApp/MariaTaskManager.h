#pragma once

#include <vector>
#include "MariaTask.h"
using namespace std;

class MariaTaskManager {
public:
	MariaTaskManager(vector<MariaTask*> *inputTaskList = NULL);
	~MariaTaskManager(void);

	bool addTask(MariaTask);
	bool addTask(string name, time_t start = NULL, time_t end = NULL);
	vector<MariaTask> findTask(string searchString);

	bool archiveTask(MariaTask);
	bool deleteArchive();

private:
	vector<MariaTask*> *taskList;
};
