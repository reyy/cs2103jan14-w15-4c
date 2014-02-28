#include "MariaTaskManager.h"

MariaTaskManager::MariaTaskManager(vector<MariaTask*> *inputTaskList){
	taskList = inputTaskList;
	if(taskList == NULL){
		taskList = new vector<MariaTask*>();
	}
}

MariaTaskManager::~MariaTaskManager(void){
	delete taskList;
}

bool MariaTaskManager::addTask(QString name, time_t start, time_t end){
	MariaTask *tempTask = new MariaTask(name, start, end);
	taskList->push_back(tempTask);
	return true;
}

vector<MariaTask*> MariaTaskManager::findTask(QString searchString){
	vector<MariaTask*> returnList;

	for(MariaTask* temp : *taskList){
		if(temp->getTitle().toStdString().find(searchString.toStdString()) != string::npos){
			returnList.push_back(temp);
		}
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::findTask(time_t start, time_t end){
	vector<MariaTask*> returnList;

	for(MariaTask* temp : *taskList){
		if(start == NULL && temp->getEnd() == end ||			 //Handle deadline tasks
			temp->getStart() >= start && temp->getEnd() <= end){ //Handle timed
				returnList.push_back(temp);
		}
	}

	return returnList;
}