#include "MariaTask.h"

MariaTask::MariaTask(string title,MariaTime* start, MariaTime* end){
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

MariaTask::MariaTask(string title, string description,MariaTime* start, MariaTime* end){
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

std::string MariaTask::getTitle(){
	return title;
}

std::string MariaTask::getDescription(){
	return description;
}

MariaTime* MariaTask::getStart(){
	return start;
}

MariaTime* MariaTask::getEnd(){
	return end;
}

double MariaTask::getDuration() {
	return difftime(end->getUnixTime(),start->getUnixTime());
}

void MariaTask::setTitle(string title){
	this->title = title;
}

void MariaTask::setDescription(string description){
	this->description = description;
}

void MariaTask::setStart(MariaTime* start){
	delete this->start;
	this->start = start;
}

void MariaTask::setEnd(MariaTime* end){
	delete this->end;
	this->end = end;
}