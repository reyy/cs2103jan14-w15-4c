#pragma once

#include <algorithm> 
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaUIStateDisplay.h"
#include "MariaUIClock.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaTaskManager;
class MariaUIStateConflict : public MariaUIStateDisplay {
public:
	static const float TASK_STARTHEIGHT_SCALE;
	static const float TASKBAR_STARTHEIGHT_SCALE;
	static const int MAX_ITEM_IN_PAGE = 3;

private:

	string _conflictTaskTitle;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();

public:
	MariaUIStateConflict(QMainWindow* qmainWindow, MariaTaskManager *taskManager, string conflictTaskTitle);
	~MariaUIStateConflict();

	void updateGUI();
};

