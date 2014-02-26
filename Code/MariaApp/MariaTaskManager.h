#pragma once
#include<vector>
#include "MariaTask.h"
using namespace std;

class MariaTaskManager{

private:
	vector<MariaTask> *taskList;	
public:
	MariaTaskManager(vector<MariaTask> *inputTaskList = NULL);
	~MariaTaskManager(void);

	bool addTask(MariaTask);
	vector<MariaTask> findTask(string searchString);

	bool archiveTask(MariaTask);
	bool deleteArchive();
};
