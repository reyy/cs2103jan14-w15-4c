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

bool MariaTaskManager::addTask(string name, time_t start, time_t end){
	MariaTask *tempTask = new MariaTask(name, start, end);
	taskList->push_back(tempTask);
	return true;
}