#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaUIStateDisplay.h"
#include "MariaTime.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaTaskManager;
class MariaUIStateShow : public MariaUIStateDisplay {
public:
	static const float TASK_STARTHEIGHT_SCALE;
	static const float TASKBAR_STARTHEIGHT_SCALE;
	static const int MAX_ITEM_IN_PAGE = 20;

	typedef enum {
		DAY, 
		MONTH, 
		YEAR
	} VIEW_TYPE;

private:
	QLabel *_titleLabel;
	VIEW_TYPE _currentViewType;
	MariaTime _targetTime;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();
public:
	MariaUIStateShow(QMainWindow* qmainWindow, MariaTaskManager *taskManager, VIEW_TYPE currentViewType, MariaTime targetTime);
	~MariaUIStateShow();
	
	void updateGUI();
};

