#pragma once

#include <QtCore/QCoreApplication>

class MariaTask{
public:
	MariaTask(void);
	~MariaTask(void);

	int		type;
	QString	title;
	QString	description;
	time_t	start;
	time_t	end;
};
