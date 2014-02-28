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

	MariaTask(string title, time_t start, time_t end);
	~MariaTask(void);

	string getTitle();
	time_t getStart();
	time_t getEnd();

private:
	int		type;
	string	title;
	string	description;
	time_t	start;
	time_t	end;
};
