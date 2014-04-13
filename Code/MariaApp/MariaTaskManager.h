//@author A0111823U
#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include "MariaTask.h"
using namespace std;

class MariaTaskManager : public MariaUndoObserver {
public:
	static const int UNDO_LIMIT = 5;

	MariaTaskManager(vector<MariaTask*> *inputTaskList = NULL);
	~MariaTaskManager(void);

	MariaTask* addTask(string name, MariaTime* start = NULL, MariaTime* end = NULL);
	vector<MariaTask*> findTask(string searchString, bool addToHistory = true);
	vector<MariaTask*> findTask(MariaTime* start, MariaTime* end, bool addToHistory = true);
	vector<MariaTask*> findTask(MariaTime* start, MariaTime* end, MariaTask::TaskType type, bool addToHistory = true);
	vector<MariaTask*> findTask(MariaTask::TaskType type, bool addToHistory = true);
	vector<MariaTask*> getWeeklyTask();
	vector<MariaTask*> getAllTasks(bool addToHistory = false);

	int compareToPreviousQuery();
	MariaTask* undoLast();

	bool archiveTask(MariaTask*); //Note: For now, this deletes tasks!
	bool deleteArchive();

	void notifyAction(MariaTaskInterface* task, bool isAddTask = false);

private:
#ifdef _DEBUG
public:
#endif
	vector<MariaTask*> *taskList;
	vector<pair<MariaTask*, MariaTask*>*> *undoList;

	vector<MariaTask*> *queryResult;
	string previousSearchString;
	MariaTime *previousStart;
	MariaTime *previousEnd;
	MariaTask::TaskType previousType;

	void deletePreviousQuery();
	void storePreviousQuery(string);
	void storePreviousQuery(MariaTask::TaskType);
	void storePreviousQuery(MariaTime*, MariaTime*);
	void storePreviousQuery(MariaTime*, MariaTime*, MariaTask::TaskType);
	

	string lowercaseString(string text);
	void sortTasks();
	static bool compareTasks(MariaTask* d1, MariaTask* d2);
};
