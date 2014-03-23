#include "MariaTask.h"

MariaTask::MariaTask(string title, MariaTime* start, MariaTime* end) {
	this->title = title;
	this->start = start;
	this->end = end;
	
	refreshTaskType();
}

MariaTask::MariaTask(string title, string description, MariaTime* start, MariaTime* end) {
	this->title = title;
	this->description = description;
	this->start = start;
	this->end = end;
	
	refreshTaskType();
}

MariaTask::~MariaTask(void) {
}

MariaTask::TaskType MariaTask::getType() {
	return type;
}

std::string MariaTask::getTitle() {
	return title;
}

std::string MariaTask::getDescription() {
	return description;
}

MariaTime* MariaTask::getStart() {
	return start;
}

MariaTime* MariaTask::getEnd() {
	return end;
}

double MariaTask::getDuration() {
	return difftime(end->get(), start->get());
}

string MariaTask::getTimeFromNow() {
	if(type == DEADLINE) {
		return getEnd()->getTimeFromNow();
	}else if(type == TIMED) {
		return getStart()->getTimeFromNow();
	}
	else return "";
}

void MariaTask::setTitle(string title) {
	this->title = title;
}

void MariaTask::setDescription(string description) {
	this->description = description;
}

void MariaTask::setStart(MariaTime* start) {
	delete this->start;
	this->start = start;
}

void MariaTask::setEnd(MariaTime* end) {
	delete this->end;
	this->end = end;
}

void MariaTask::refreshTaskType() {
	if(start == NULL && end == NULL) {
		type = TaskType::FLOATING;
	}else if(start == NULL) {
		type = TaskType::DEADLINE;
	} else {
		type = TaskType::TIMED;
	}
}

bool MariaTask::operator<(MariaTask rhs) { 
	
	if(type == rhs.getType()){
		if(type == MariaTask::TIMED){
			return start->get() < rhs.getStart()->get();
		}else if(type == MariaTask::DEADLINE){
			return end->get() < rhs.getEnd()->get();
		}else{
			return title < rhs.getTitle(); 
		}
	}else{
		return type < getType();
	}
	
}