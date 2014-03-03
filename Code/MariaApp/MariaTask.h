#pragma once

#include <string>

using namespace std;

class MariaTask{
public:
	typedef enum {
		FLOATING,
		DEADLINE,
		TIMED
	} TaskType;
	
	MariaTask(std::string title, time_t start, time_t end);
	MariaTask(std::string title, std::string description,time_t start, time_t end);
	~MariaTask();
	
	
	TaskType getType();
	std::string getTitle();
	std::string getDescription();
	time_t getStart();
	time_t getEnd();

private:
	TaskType	type;
	std::string		title;
	std::string		description;
	time_t		start;
	time_t		end;
};
