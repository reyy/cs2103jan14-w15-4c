#include "MariaTaskManager.h"
#include <assert.h>

MariaTaskManager::MariaTaskManager(vector<MariaTask*> *inputTaskList) {
	taskList = inputTaskList;
	if(taskList == NULL) {
		taskList = new vector<MariaTask*>();
	}
	sortTasks();

	undoList = new vector<pair<MariaTask**,MariaTask*>*>();
}

MariaTaskManager::~MariaTaskManager(void) {
	for(MariaTask* t: (*this->taskList)) {
		delete t;
	}
	delete taskList;

	for(pair<MariaTask**,MariaTask*>* t: (*this->undoList)) {
		delete t;
	}
	delete undoList;
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

void MariaTaskManager::sortTasks() {
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

bool MariaTaskManager::compareTasks(MariaTask* t1, MariaTask* t2) {
	return ((*t1) < (*t2));
}

bool MariaTaskManager::undoLast() {
	if(undoList->empty()) {
		return false;
	}
	MariaTask* taskPointer = *(undoList->back())->first;
	MariaTask* oldTask = undoList->back()->second;
	auto it = std::find(taskList->begin(), taskList->end(), taskPointer);
	
	if(it != taskList->end()) {
		if(oldTask == NULL) { 
			//last action = addTask
			delete taskPointer;
			taskList->erase(it);
		} else { 
			//last action = editTask
			delete taskPointer; //delete current task
			*(undoList->back())->first = oldTask; //set pointer in taskList to old task
		}
	} else {
		//last action = deleteTask
		taskList->push_back(oldTask);
		sortTasks();
	}
	undoList->pop_back();
	return true;
}

void MariaTaskManager::notifyAction(MariaTask* task, bool isAddTask) {
	
	MariaTask *oldTask = NULL;
	MariaTask **taskPointer = NULL;
	
	for(int i = 0; i<taskList->size(); i++) {
		if(task == (*taskList)[i]){
			taskPointer = &(*taskList)[i];
			break;
		}
	}

	assert(taskPointer != NULL);

	if(!isAddTask) {
		oldTask = task->getClone();
	} 

	while(undoList->size() >= UNDO_LIMIT) {
		undoList->erase(undoList->begin());
	}

	undoList->push_back(new pair<MariaTask**, MariaTask*>(taskPointer, oldTask));
}
