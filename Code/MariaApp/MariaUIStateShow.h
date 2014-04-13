#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaUIStateDisplay.h"
#include "MariaTime.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaUIStateShow : public MariaUIStateDisplay {
public:
	static const int MAX_ITEM_IN_PAGE = 5;

	MariaUIStateShow(QMainWindow* qmainWindow, string title, vector<MariaTask*> listOfTasks);
	~MariaUIStateShow();
	
	void updateGUI();

private:
	static const int BACKGROUND_R = 116;
	static const int BACKGROUND_G = 30;
	static const int BACKGROUND_B = 168;
	static const float TASK_STARTHEIGHT_SCALE;
	static const float TASKBAR_STARTHEIGHT_SCALE;
	static const float TITLE_WIDTH;
	static const float TITLE_HEIGHT;
	static const float TITLE_HEIGHT_SCALE;

	QLabel *_titleLabel;
	vector<MariaTask*> _listOfTasks;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();
};

