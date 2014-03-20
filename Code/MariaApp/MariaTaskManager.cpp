#include "MariaTaskManager.h"
#include <assert.h>

MariaTaskManager::MariaTaskManager(vector<MariaTask*> *inputTaskList) {
	taskList = inputTaskList;
	if(taskList == NULL) {
		taskList = new vector<MariaTask*>();
	}
	sortTasks();
}

MariaTaskManager::~MariaTaskManager(void) {
	for(MariaTask* t: (*this->taskList)) {
		delete t;
	}
	delete taskList;
}

MariaTask* MariaTaskManager::addTask(string name, MariaTime* start, MariaTime* end) {
	MariaTask *tempTask = new MariaTask(name, start, end);
	taskList->push_back(tempTask);

	sortTasks();
	return tempTask;
}

vector<MariaTask*> MariaTaskManager::findTask(std::string searchString) {
	vector<MariaTask*> returnList;
	searchString = lowercaseString(searchString);

	for(MariaTask* temp : *taskList) {
		if(lowercaseString(temp->getTitle()).find(searchString) != string::npos) {
			returnList.push_back(temp);
		}
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::findTask(MariaTime* start, MariaTime* end, MariaTask::TaskType type) {
	vector<MariaTask*> returnList;

	for(MariaTask* temp : *taskList) {
		if(temp->getType() == type) {
		switch(type) {
			case MariaTask::TaskType::DEADLINE:
				if(temp->getEnd()->get() >= start->get() && temp->getEnd()->get() <= end->get()) {
					returnList.push_back(temp);
				}
				break;
			case MariaTask::TaskType::TIMED:
				if(temp->getStart()->get() >= start->get() && temp->getStart()->get() <= end->get()) {
					returnList.push_back(temp);
				}
				break;
			}
		}
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::findTask(MariaTask::TaskType type) {
	vector<MariaTask*> returnList;

	for(MariaTask* temp : *taskList) {
		if(temp->getType() == type) {
			returnList.push_back(temp);
		}
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::getAllTasks() {
	assert(taskList!=NULL);

	return *taskList;
}

vector<MariaTask*> MariaTaskManager::getAllTasks(MariaTask::TaskType type) {
	assert(taskList!=NULL);
	vector<MariaTask*> returnList;

	for(MariaTask *temp : *taskList) {
		if(temp->getType() == type) {
			returnList.push_back(temp);
		}
	}
	return returnList;
}

bool MariaTaskManager::archiveTask(MariaTask* task){
	assert(task!=NULL);
	
	auto it = std::find(taskList->begin(), taskList->end(), task);

	if(it != taskList->end()) {
		delete (*it);
		taskList->erase(it);
		return true;
	} else {
		return false;
	}
	
}

string MariaTaskManager::lowercaseString(string text) {
	string toReturn = "";

	for (int i = 0; i < text.size(); i++ ) {
		toReturn += tolower(text[i]);
	}

	return toReturn;
}

void MariaTaskManager::sortTasks(){
	vector<MariaTask*> floatingTasks = getAllTasks(MariaTask::FLOATING);
	vector<MariaTask*> deadlineTasks = getAllTasks(MariaTask::DEADLINE);
	vector<MariaTask*> timedTasks = getAllTasks(MariaTask::TIMED);
	std::sort(floatingTasks.begin(), floatingTasks.end(), &compareTasks);
	std::sort(deadlineTasks.begin(), deadlineTasks.end(), &compareTasks);
	std::sort(timedTasks.begin(), timedTasks.end(), &compareTasks);
	delete taskList;
	taskList = new vector<MariaTask*>();
	taskList->insert(taskList->end(), floatingTasks.begin(), floatingTasks.end());
	taskList->insert(taskList->end(), deadlineTasks.begin(), deadlineTasks.end());
	taskList->insert(taskList->end(), timedTasks.begin(), timedTasks.end());
}

bool MariaTaskManager::compareTasks(MariaTask* t1, MariaTask* t2){
	return ((*t1) < (*t2));
}