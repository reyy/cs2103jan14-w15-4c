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
	static const int MAX_ITEM_IN_PAGE = 5;
	static const float TITLE_WIDTH;
	static const float TITLE_HEIGHT;
	static const float TITLE_HEIGHT_SCALE;

private:
	QLabel *_titleLabel;
	vector<MariaTask*> _listOfTasks;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();

public:
	MariaUIStateShow(QMainWindow* qmainWindow, MariaTaskManager *taskManager, string title, vector<MariaTask*> listOfTasks);
	~MariaUIStateShow();
	
	void updateGUI();
};

