#include "MariaTaskManager.h"

MariaTaskManager::MariaTaskManager(vector<MariaTask*> *inputTaskList){
	taskList = inputTaskList;
	if(taskList == NULL){
		taskList = new vector<MariaTask*>();
	}
}

MariaTaskManager::~MariaTaskManager(void){
	for(MariaTask* t: (*this->taskList)){
		delete t;
	}
	delete taskList;
}

MariaTask* MariaTaskManager::addTask(string name, MariaTime* start, MariaTime* end){
	MariaTask *tempTask = new MariaTask(name, start, end);
	taskList->push_back(tempTask);
	return tempTask;
}

vector<MariaTask*> MariaTaskManager::findTask(std::string searchString){
	vector<MariaTask*> returnList;
	searchString = lowercaseString(searchString);

	for(MariaTask* temp : *taskList){
		if(lowercaseString(temp->getTitle()).find(searchString) != string::npos){
			returnList.push_back(temp);
		}
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::findTask(MariaTime* start, MariaTime* end){
	vector<MariaTask*> returnList;

	for(MariaTask* temp : *taskList){
		if(start == NULL && temp->getEnd() == end ||			 //Handle deadline tasks
			temp->getStart() >= start && temp->getEnd() <= end){ //Handle timed
				returnList.push_back(temp);
		}
	}

	return returnList;
}

vector<MariaTask*> MariaTaskManager::getAllTasks(){
	return *this->taskList;
}

bool MariaTaskManager::archiveTask(MariaTask* task){
	auto it = std::find(taskList->begin(), taskList->end(), task);

	if(it != taskList->end()){
		delete (*it);
		taskList->erase(it);
		return true;
	}else{
		return false;
	}
	
}

string MariaTaskManager::lowercaseString(string text) {
	string toReturn = "";

	for (int i = 0; i < text.size(); i++) {
		toReturn += tolower(text[i]);
	}

	return toReturn;
}
