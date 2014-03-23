#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include "MariaTask.h"
using namespace std;

class MariaTaskManager {
public:
	static const int UNDO_LIMIT = 5;

	MariaTaskManager(vector<MariaTask*> *inputTaskList = NULL);
	~MariaTaskManager(void);

	MariaTask* addTask(string name, MariaTime* start = NULL, MariaTime* end = NULL);
	vector<MariaTask*> findTask(string searchString);
	vector<MariaTask*> findTask(MariaTime* start, MariaTime* end, MariaTask::TaskType type);
	vector<MariaTask*> findTask(MariaTask::TaskType type);
	vector<MariaTask*> getAllTasks(MariaTask::TaskType);
	vector<MariaTask*> getAllTasks();

	bool undoLast();

	bool archiveTask(MariaTask*); //Note: For now, this deletes tasks!
	bool deleteArchive();

private:
#ifdef _DEBUG
public:
#endif
	vector<MariaTask*> *taskList;
	vector<pair<MariaTask**, MariaTask*>*> *undoList;
	void notifyAction(MariaTask* task, bool isAddTask = false);

	string lowercaseString(string text);
	void sortTasks();
	static bool compareTasks(MariaTask* d1, MariaTask* d2);
};
