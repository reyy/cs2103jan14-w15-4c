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

MariaTask* MariaTaskManager::addTask(string name, MariaTime* start, MariaTime* end){
	MariaTask *tempTask = new MariaTask(name, start, end);
	taskList->push_back(tempTask);
	return tempTask;
}

vector<MariaTask*> MariaTaskManager::findTask(std::string searchString){
	vector<MariaTask*> returnList;

	for(MariaTask* temp : *taskList){
		if(temp->getTitle().find(searchString) != string::npos){
			returnList.push_back(temp);
		}
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::findTask(MariaTime* start, MariaTime* end){
	vector<MariaTask*> returnList;

	//for(MariaTask* temp : *taskList){
	//	if(start == NULL && temp->getEnd() == end ||			 //Handle deadline tasks
	//		temp->getStart() >= start && temp->getEnd() <= end){ //Handle timed
	//			returnList.push_back(temp);
	//	}
	//}

	return returnList;
}