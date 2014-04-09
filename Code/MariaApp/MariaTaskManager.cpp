#include "MariaTaskManager.h"
#include <assert.h>

MariaTaskManager::MariaTaskManager(vector<MariaTask*> *inputTaskList) {
	taskList = inputTaskList;
	if(taskList == NULL) {
		taskList = new vector<MariaTask*>();
	}
	sortTasks();

	undoList = new vector<pair<MariaTask*,MariaTask*>*>();
	MariaTask::initObserver(this);

	previousStart = NULL;
	previousEnd = NULL;
	queryResult = NULL;
}

MariaTaskManager::~MariaTaskManager(void) {
	for(MariaTask* t: (*this->taskList)) {
		delete t;
	}
	delete taskList;

	for(pair<MariaTask*,MariaTask*>* t: (*this->undoList)) {
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
		if(queryResult != NULL) {
			delete queryResult;
		}
		queryResult = new vector<MariaTask*> (returnList);
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
		if(queryResult != NULL) {
			delete queryResult;
		}
		queryResult = new vector<MariaTask*> (returnList);
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
		if(queryResult != NULL) {
			delete queryResult;
		}
		queryResult = new vector<MariaTask*> (returnList);
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
		if(queryResult != NULL) {
			delete queryResult;
		}
		queryResult = new vector<MariaTask*> (returnList);
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::getWeeklyTask() {
	//Manually set start and end time to return the task for the next 7 days from now.
	MariaTime* startTime = &MariaTime::getCurrentTime();
	startTime->setHour(0);
	startTime->setMin(0);
	MariaTime* endTime =  &MariaTime::getCurrentTime();
	endTime->setDay(startTime->getDay()+7);
	endTime->setHour(23);
	endTime->setMin(59);
	return findTask(startTime,endTime);
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

int MariaTaskManager::compareToPreviousQuery() {
	if(queryResult == NULL) {
		return false;
	}
	vector<MariaTask*> previousQueryResult = vector<MariaTask*>(*queryResult);

	if(previousSearchString != "") {
		findTask(previousSearchString);
	} else if(previousStart == NULL && previousEnd == NULL) {
		findTask(previousType);
	} else if(previousType != MariaTask::TaskType::INVALID) {
		findTask(previousStart, previousEnd, previousType);
	} else {
		findTask(previousStart, previousEnd);
	}

	return queryResult->size() - previousQueryResult.size();
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
	MariaTime * tempStart = new MariaTime(previousStart->get());
	MariaTime * tempEnd = new MariaTime(previousEnd->get());
	deletePreviousQuery();
	this->previousStart = tempStart;
	this->previousEnd = tempEnd;
}

void MariaTaskManager::storePreviousQuery(MariaTime* previousStart, MariaTime* previousEnd, MariaTask::TaskType previousType) {
	MariaTime * tempStart = new MariaTime(previousStart->get());
	MariaTime * tempEnd = new MariaTime(previousEnd->get());
	deletePreviousQuery();
	this->previousStart = tempStart;
	this->previousEnd = tempEnd;
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

MariaTask* MariaTaskManager::undoLast() {
	if(undoList->empty()) {
		return NULL;
	}
	MariaTask* toReturn = NULL;
	MariaTask* taskPointer = (undoList->back())->first;
	MariaTask* oldTask = undoList->back()->second;
	auto it = std::find(taskList->begin(), taskList->end(), taskPointer);

	if(it != taskList->end()) {
		toReturn = (undoList->back())->first;
		if(oldTask == NULL) { 
			//last action = addTask
			delete taskPointer;
			taskList->erase(it);
		} else { 
			//last action = editTask
			/*delete taskPointer; //delete current task
			taskList->erase(it);
			for(pair<MariaTask*,MariaTask*> *p : *undoList) {
			if(p->first == taskPointer) {
			p->first = oldTask;
			}
			}
			taskList->push_back(oldTask);
			sortTasks();*/
			taskPointer->setAll(oldTask);
			delete oldTask;
			//(undoList->back())->first = oldTask; //set pointer in taskList to old task
		}
	} else {
		//last action = deleteTask
		for(pair<MariaTask*,MariaTask*> *p : *undoList) {
			if(p->first == taskPointer) {
				p->first = oldTask;
			}
		}
		taskList->push_back(oldTask);
		sortTasks();

		toReturn = oldTask;
	}
	undoList->pop_back();
	return toReturn;
}

void MariaTaskManager::notifyAction(MariaTaskInterface* task, bool isAddTask) {

	MariaTask *oldTask = NULL;
	MariaTask *taskPointer = NULL;

	for(int i = 0; i<taskList->size(); i++) {
		if(((MariaTask*)task) == (*taskList)[i]){
			taskPointer = (*taskList)[i];
			break;
		}
	}
	//taskPointer = (MariaTask*)task;

	assert(taskPointer != NULL);

	if(!isAddTask) {
		oldTask = ((MariaTask*)task)->getClone();
	} 

	while(undoList->size() >= UNDO_LIMIT) {
		undoList->erase(undoList->begin());
	}

	undoList->push_back(new pair<MariaTask*, MariaTask*>(taskPointer, oldTask));
}
