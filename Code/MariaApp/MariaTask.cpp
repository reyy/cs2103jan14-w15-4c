//@author A0111823U
#include "MariaTask.h"

MariaTask::MariaTask(string title, MariaTime* start, MariaTime* end) {
	this->title = title;
	this->start = start;
	this->end = end;
	this->isDone = false;

	this->created = new MariaTime(MariaTime::getCurrentTime());
	
	refreshTaskType();
}

MariaTask::MariaTask(string title, string description, MariaTime* start, MariaTime* end, bool isDone) {
	this->title = title;
	this->description = description;
	this->start = start;
	this->end = end;
	this->isDone = isDone;

	this->created = new MariaTime(MariaTime::getCurrentTime());
	
	refreshTaskType();
}

MariaTask::~MariaTask(void) {
	//delete start;
	//delete end;
}

MariaTask::TaskType MariaTask::getType() {
	return type;
}

string MariaTask::getTitle() {
	return title;
}

string MariaTask::getDescription() {
	return description;
}

MariaTime* MariaTask::getStart() {
	return start;
}

MariaTime* MariaTask::getEnd() {
	return end;
}

MariaTime* MariaTask::getCreated() {
	return created;
}

bool MariaTask::getIsDone() {
	return isDone;
}

double MariaTask::getDuration() {
	return difftime(end->get(), start->get());
}

MariaTask* MariaTask::getClone() {
	MariaTime *clonedStart = NULL;
	MariaTime *clonedEnd = NULL;

	if(start != NULL) {
		clonedStart = new MariaTime(start->get());
	}
	if(end != NULL) {
		clonedEnd = new MariaTime(end->get());
	}

	MariaTask *clonedTask = new MariaTask(title, description, clonedStart, clonedEnd, isDone);
	clonedTask->setCreated(new MariaTime(created->get()));
	return clonedTask;
}

string MariaTask::getTimeFromNow() {
	if(type == DEADLINE) {
		return getEnd()->getTimeFromNow();
	}else if(type == TIMED) {
		return getStart()->getTimeFromNow();
	}
	return "";
}

void MariaTask::setTitle(string title) {
	if(MariaTask::observer != NULL){
		MariaTask::observer->notifyAction(this);
	}
	this->title = title;
}

void MariaTask::setDescription(string description) {
	if(MariaTask::observer != NULL){
		MariaTask::observer->notifyAction(this);
	}
	this->description = description;
}

void MariaTask::setStart(MariaTime* start) {
	if(MariaTask::observer != NULL){
		MariaTask::observer->notifyAction(this);
	}
	delete this->start;
	this->start = start;
	refreshTaskType();
}

void MariaTask::setEnd(MariaTime* end) {
	if(MariaTask::observer != NULL){
		MariaTask::observer->notifyAction(this);
	}
	delete this->end;
	this->end = end;
	refreshTaskType();
}

void MariaTask::setCreated(MariaTime* created) {
	delete this->created;
	this->created = created;
}

void MariaTask::setIsDone(bool isDone) {
	if(MariaTask::observer != NULL){
		MariaTask::observer->notifyAction(this);
	}
	this->isDone = isDone;
}

void MariaTask::setAll(MariaTask* other) {
	title = other->getTitle();
	description = other->getDescription();
	isDone = (other->getIsDone());

	delete start;
	if(other->getStart() != NULL) {
		start = new MariaTime(other->getStart()->get());
	} else {
		start = NULL;
	}
	
	delete end;
	if(other->getEnd() != NULL) {
		end = new MariaTime(other->getEnd()->get());
	} else {
		end = NULL;
	}
	refreshTaskType();
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

void MariaTask::initObserver(MariaUndoObserver* observer) {
	MariaTask::observer = observer;
}

MariaUndoObserver *MariaTask::observer = NULL;