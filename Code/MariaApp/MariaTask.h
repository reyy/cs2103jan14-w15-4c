#pragma once

#include <functional>
#include <string>
#include "MariaTime.h"
#include "MariaTaskManager.h"
using namespace std;



class MariaTask{
public:
	typedef void (MariaTaskManager::*notifyAction)(MariaTask*, bool);
	typedef std::function<void(MariaTask*, bool)> observerFunction;
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
	string getTimeFromNow();
	MariaTime* getStart();
	MariaTime* getEnd();
	double getDuration();
	MariaTask* getClone();
	
	void setTitle(string);
	void setDescription(string);
	void setStart(MariaTime*);
	void setEnd(MariaTime*);

	void refreshTaskType();

	bool operator<(MariaTask rhs);
	static void initObserver(notifyAction observerFunc);
	static notifyAction notifyObserver;

private:
	TaskType	type;
	string		title;
	string		description;
	MariaTime*	start;
	MariaTime*	end;

};
