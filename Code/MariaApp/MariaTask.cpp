#include "MariaTask.h"

MariaTask::MariaTask(QString title,time_t start, time_t end){
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

MariaTask::MariaTask(QString title, QString description,time_t start, time_t end){
	this->title = title;
	this->description=description;
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

MariaTask::TaskType MariaTask::getType() {
	return type;
}

QString MariaTask::getTitle(){
	return title;
}

QString MariaTask::getDescription(){
	return description;
}

time_t MariaTask::getStart(){
	return start;
}

time_t MariaTask::getEnd(){
	return end;
}