#include "MariaTaskManager.h"
#include <assert.h>

MariaTaskManager::MariaTaskManager(vector<MariaTask*> *inputTaskList) {
	taskList = inputTaskList;
	if(taskList == NULL) {
		taskList = new vector<MariaTask*>();
	}
	sortTasks();

	undoList = new vector<pair<MariaTask**,MariaTask*>*>();
	MariaTask::initObserver(this);

	previousStart = NULL;
	previousEnd = NULL;
	previousQueryResult = NULL;
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
	tempTask->initObserver(this);
	taskList->push_back(tempTask);

	sortTasks();

	this->notifyAction(tempTask, true);

	return tempTask;
}

vector<MariaTask*> MariaTaskManager::findTask(std::string searchString, bool addToHistory) {
	vector<MariaTask*> returnList;
	searchString = lowercaseString(searchString);

	for(MariaTask* temp : *taskList) {
		if(lowercaseString(temp->getTitle()).find(searchString) != string::npos) {
			returnList.push_back(temp);
		}
	}

	if(addToHistory) {
		storePreviousQuery(searchString);
		if(previousQueryResult != NULL) {
			delete previousQueryResult;
		}
		previousQueryResult = new vector<MariaTask*> (returnList);
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::findTask(MariaTime* start, MariaTime* end, bool addToHistory) {
	vector<MariaTask*> returnList;

	for(MariaTask* temp : *taskList) {
		if (temp->getEnd() == NULL) {
			continue;
		}

		if(temp->getEnd()->get() >= start->get() && temp->getEnd()->get() <= end->get()) {
			returnList.push_back(temp);
		}
	}

	if(addToHistory) {
		storePreviousQuery(start, end);
		if(previousQueryResult != NULL) {
			delete previousQueryResult;
		}
		previousQueryResult = new vector<MariaTask*> (returnList);
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::findTask(MariaTime* start, MariaTime* end, MariaTask::TaskType type, bool addToHistory) {
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

	if(addToHistory) {
		storePreviousQuery(start, end, type);
		if(previousQueryResult != NULL) {
			delete previousQueryResult;
		}
		previousQueryResult = new vector<MariaTask*> (returnList);
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::findTask(MariaTask::TaskType type, bool addToHistory) {
	assert(taskList!=NULL);
	vector<MariaTask*> returnList;

	for(MariaTask *temp : *taskList) {
		if(temp->getType() == type) {
			returnList.push_back(temp);
		}
	}

	if(addToHistory) {
		storePreviousQuery(type);
		if(previousQueryResult != NULL) {
			delete previousQueryResult;
		}
		previousQueryResult = new vector<MariaTask*> (returnList);
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::getAllTasks() {
	assert(taskList!=NULL);

	return *taskList;
}

bool MariaTaskManager::archiveTask(MariaTask* task) {
	assert(task!=NULL);

	this->notifyAction(task);
	
	auto it = std::find(taskList->begin(), taskList->end(), task);

	if(it != taskList->end()) {
		delete (*it);
		taskList->erase(it);
		return true;
	} else {
		return false;
	}
	
}

bool MariaTaskManager::comparePreviousQuery() {
	if(previousQueryResult == NULL) {
		return true;
	}
	vector<MariaTask*> newList;

	if(previousSearchString != "") {
		newList = findTask(previousSearchString, false);
	} else if(previousStart == NULL && previousEnd == NULL) {
		newList = findTask(previousType, false);
	} else if(previousType != MariaTask::TaskType::INVALID) {
		newList = findTask(previousStart, previousEnd, previousType, false);
	} else {
		newList = findTask(previousStart, previousEnd, false);
	}

	if(newList.size() != previousQueryResult->size()) {
		return false;
	}

	return true;	
}

void MariaTaskManager::deletePreviousQuery() {
	previousSearchString = "";
	if(previousStart != NULL) {
		delete previousStart;
	}
	previousStart = NULL;
	if(previousEnd != NULL) {
		delete previousEnd;
	}
	previousEnd = NULL;
	previousType = MariaTask::TaskType::INVALID;
}

void MariaTaskManager::storePreviousQuery(string previousSearchString) {
	deletePreviousQuery();
	this->previousSearchString = previousSearchString;
}

void MariaTaskManager::storePreviousQuery(MariaTask::TaskType previousType) {
	deletePreviousQuery();
	this->previousType = previousType;
}

void MariaTaskManager::storePreviousQuery(MariaTime* previousStart, MariaTime* previousEnd){
	deletePreviousQuery();
	this->previousStart = new MariaTime(previousStart->get());
	this->previousEnd = new MariaTime(previousEnd->get());
}

void MariaTaskManager::storePreviousQuery(MariaTime* previousStart, MariaTime* previousEnd, MariaTask::TaskType previousType) {
	deletePreviousQuery();
	this->previousStart = new MariaTime(previousStart->get());
	this->previousEnd = new MariaTime(previousEnd->get());
	this->previousType = previousType;
}


string MariaTaskManager::lowercaseString(string text) {
	string toReturn = "";

	for (int i = 0; i < text.size(); i++ ) {
		toReturn += tolower(text[i]);
	}

	return toReturn;
}

void MariaTaskManager::sortTasks() {
	vector<MariaTask*> floatingTasks = findTask(MariaTask::FLOATING, false);
	vector<MariaTask*> deadlineTasks = findTask(MariaTask::DEADLINE, false);
	vector<MariaTask*> timedTasks = findTask(MariaTask::TIMED, false);
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

void MariaTaskManager::notifyAction(MariaTaskInterface* task, bool isAddTask) {
	
	MariaTask *oldTask = NULL;
	MariaTask **taskPointer = NULL;
	
	for(int i = 0; i<taskList->size(); i++) {
		if(((MariaTask*)task) == (*taskList)[i]){
			taskPointer = &(*taskList)[i];
			break;
		}
	}

	assert(taskPointer != NULL);

	if(!isAddTask) {
		oldTask = ((MariaTask*)task)->getClone();
	} 

	while(undoList->size() >= UNDO_LIMIT) {
		undoList->erase(undoList->begin());
	}

	undoList->push_back(new pair<MariaTask**, MariaTask*>(taskPointer, oldTask));
}
