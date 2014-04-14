#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaUIStateDisplay.h"
#include "MariaUIClock.h"
#include "MariaUITask.h"
#include "MariaTask.h"
#include "MariaUIPreview.h"

class MariaUIStateHome : public MariaUIStateDisplay {
public:
	static const int MAX_ITEM_IN_PAGE = 5;

	MariaUIStateHome(QMainWindow* qmainWindow, vector<MariaTask*> weekTask);
	~MariaUIStateHome();

	void updateGUI();
	MariaUIPreview* getUIPreview();

private:
	static const int BACKGROUND_R = 114;
	static const int BACKGROUND_G = 143;
	static const int BACKGROUND_B = 22;
	static const float TASK_STARTHEIGHT_SCALE;
	static const float TASKBAR_STARTHEIGHT_SCALE;

	MariaUIPreview *_preview;
	MariaUIClock *_clock;
	vector<MariaTask*> _weekTask;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();
};

