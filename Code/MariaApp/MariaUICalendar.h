#pragma once
#include "mariauirolling.h"

class MariaUICalendar : MariaUIRolling {
public:
	enum VIEW_TYPE {
		DAY, WEEK, MONTH, YEAR
	};
private:
public:
	MariaUICalendar(QMainWindow *parent);
	~MariaUICalendar();
};

