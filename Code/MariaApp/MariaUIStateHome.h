#pragma once

#include <algorithm> 
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaUIStateDisplay.h"
#include "MariaUIClock.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaTaskManager;
class MariaUIStateHome : public MariaUIStateDisplay {
public:
	static const float TASK_STARTHEIGHT_SCALE;
	static const float TASKBAR_STARTHEIGHT_SCALE;
	static const int MAX_ITEM_IN_PAGE = 20;

private:
	MariaUIClock *_clock;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();

public:
	MariaUIStateHome(QMainWindow* qmainWindow, MariaTaskManager *taskManager);
	~MariaUIStateHome();
};

