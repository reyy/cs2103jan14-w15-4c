#pragma once

#include <algorithm> 
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaUIStateDisplay.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaUIStateConflict : public MariaUIStateDisplay {
public:
	static const int BACKGROUND_R = 255;
	static const int BACKGROUND_G = 0;
	static const int BACKGROUND_B = 0;
	static const int MAX_ITEM_IN_PAGE = 3;
	static const float TASK_STARTHEIGHT_SCALE;
	static const float TASKBAR_STARTHEIGHT_SCALE;

	MariaUIStateConflict(QMainWindow* qmainWindow, vector<MariaTask*> conflictedTask);
	~MariaUIStateConflict();

	void updateGUI();

private:
	vector<MariaTask*> _conflictedTask;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();
};

