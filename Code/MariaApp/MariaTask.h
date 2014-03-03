#pragma once

#include <string>
#include <QtCore/QCoreApplication>

using namespace std;

class MariaTask{
public:
	typedef enum {
		FLOATING,
		DEADLINE,
		TIMED
	} TaskType;
	
	MariaTask(string title, time_t start, time_t end);
	MariaTask(string title, string description,time_t start, time_t end);
	~MariaTask();
	
	
	TaskType getType();
	string getTitle();
	string getDescription();
	time_t getStart();
	time_t getEnd();

private:
	TaskType	type;
	string		title;
	string		description;
	time_t		start;
	time_t		end;
};
