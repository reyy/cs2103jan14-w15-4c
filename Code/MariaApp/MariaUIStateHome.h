#pragma once

#include <algorithm> 
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"
#include "MariaUIClock.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaTaskManager;
class MariaUIStateHome : public MariaStateObject {
public:
	static const int TEXTBOX_X_OFFSET=30;
	static const float TASKBAR_STARTHEIGHT_SCALE;
	static const float TASK_STARTHEIGHT_SCALE;
	static const float TASK_STARTHEIGHT_DIFFERENCE;

private:
	QMainWindow *_qmainWindow;
	MariaTaskManager* _taskManager;

	MariaUIClock *_clock;
	std::vector<MariaUITask*> _taskStack;
	std::vector<MariaUITask*> _taskDisposeStack;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();

	void updateNumber();

	static bool sortTaskFunction(MariaUITask *i,MariaUITask *j);
public:
	MariaUIStateHome(MariaTaskManager *taskManager,QMainWindow* qmainWindow);
	~MariaUIStateHome();

	MariaUITask* addTask(MariaTask *task);
	void updateTask();
	MariaUITask* eraseTask(int index);
	MariaUITask* eraseTask(MariaTask* task);
	void clearTask();
	void sortTask();
	int getTotalTask();
};

