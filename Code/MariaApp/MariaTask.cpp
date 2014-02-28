#include "MariaTask.h"

MariaTask::MariaTask(string title, time_t start, time_t end){
	this->title = title;
	this->start = start;
	this->end = end;
	
	if(start == NULL && end == NULL){
		type = TaskType::FLOATING;
	}else if(start == NULL){
		type = TaskType::DEADLINE;
	}else{
		type = TaskType::TIMED;
	}
}
MariaTask::~MariaTask(void){
}