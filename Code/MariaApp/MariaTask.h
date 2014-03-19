#pragma once

#include <string>
#include "MariaTime.h"
using namespace std;

class MariaTask{
public:
	typedef enum {
		FLOATING, 
		DEADLINE, 
		TIMED
	} TaskType;
	
	MariaTask(string title = "", MariaTime *start = NULL, MariaTime* end = NULL);
	MariaTask(string title, string description, MariaTime* start, MariaTime* end);
	~MariaTask();
	
	
	TaskType getType();
	string getTitle();
	string getDescription();
	MariaTime* getStart();
	MariaTime* getEnd();
	double getDuration();
	string getTimeFromNow();
	
	void setTitle(string);
	void setDescription(string);
	void setStart(MariaTime*);
	void setEnd(MariaTime*);

	void refreshTaskType();

private:
	TaskType	type;
	string		title;
	string		description;
	MariaTime*	start;
	MariaTime*	end;
};
