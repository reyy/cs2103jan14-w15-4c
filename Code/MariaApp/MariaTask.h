#pragma once

#include <QtCore/QCoreApplication>

class MariaTask{
public:
	typedef enum {
		FLOATING,
		DEADLINE,
		TIMED
	} TaskType;
	
	MariaTask(QString title, time_t start, time_t end);
	MariaTask(QString title, QString description,time_t start, time_t end);
	~MariaTask();
	
	
	TaskType getType();
	QString getTitle();
	QString getDescription();
	time_t getStart();
	time_t getEnd();

private:
	TaskType	type;
	QString		title;
	QString		description;
	time_t		start;
	time_t		end;
};
