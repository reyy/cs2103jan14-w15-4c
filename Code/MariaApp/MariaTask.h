#pragma once

#include <QtCore/QCoreApplication>

class MariaTask{
public:
	typedef enum {
		FLOATING,
		DEADLINE,
		TIMED
	} TaskType;

	MariaTask();
	~MariaTask(void);
	
	
	int getType();
	QString getTitle();
	QString getDescription();
	time_t getStart();
	time_t getEnd();

private:
	int		type;
	QString	title;
	QString	description;
	time_t	start;
	time_t	end;
};
