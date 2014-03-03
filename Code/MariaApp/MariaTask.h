#pragma once

#include <string>
#include <QtCore/QCoreApplication>
#include "MariaTime.h"
using namespace std;

class MariaTask{
public:
	typedef enum {
		FLOATING,
		DEADLINE,
		TIMED
	} TaskType;
	
	MariaTask(string title, MariaTime start, MariaTime end);
	MariaTask(string title, string description,MariaTime start, MariaTime end);
	~MariaTask();
	
	
	TaskType getType();
	string getTitle();
	string getDescription();
	MariaTime getStart();
	MariaTime getEnd();
	double getDuration();
	

private:
	TaskType	type;
	string		title;
	string		description;
	MariaTime		start;
	MariaTime		end;
};
