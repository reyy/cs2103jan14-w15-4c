#pragma once

#include <algorithm> 
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"
#include "MariaUIClock.h"
#include "MariaUITaskExpanded.h"
#include "MariaTask.h"

class MariaTaskManager;
class MariaUIStateConflict : public MariaStateObject {
public:
	static const int TEXTBOX_X_OFFSET=30;
	static const float TASKBAR_STARTHEIGHT_SCALE;
	static const float TASK_STARTHEIGHT_SCALE;
	static const float TASK_STARTHEIGHT_DIFFERENCE;

private:
	QMainWindow *_qmainWindow;
	MariaTaskManager* _taskManager;

	string _conflictTaskTitle;
	std::vector<MariaUITaskExpanded*> _taskStack;
	std::vector<MariaUITaskExpanded*> _taskDisposeStack;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();

	void updateNumber();

	static bool sortTaskFunction(MariaUITaskExpanded *i,MariaUITaskExpanded *j);
public:
	MariaUIStateConflict(string conflictTaskTitle, MariaTaskManager *taskManager,QMainWindow* qmainWindow);
	~MariaUIStateConflict();

	MariaUITaskExpanded* addTask(MariaTask *task);
	void updateTask();
	MariaUITaskExpanded* eraseTask(int index);
	MariaUITaskExpanded* eraseTask(MariaTask* task);
	void clearTask();
	void sortTask();
	int getTotalTask();
};

