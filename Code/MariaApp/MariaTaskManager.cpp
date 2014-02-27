#include "MariaTaskManager.h"


MariaTaskManager::MariaTaskManager(vector<MariaTask> *inputTaskList){
	taskList = inputTaskList;
	if(taskList == NULL){
		taskList = new vector<MariaTask>();
	}
}


MariaTaskManager::~MariaTaskManager(void){
	delete taskList;
}
