#pragma once

#include <algorithm> 
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaTaskManager;
class MariaUIStateDisplay : public MariaStateObject {
public:
	static const int TEXTBOX_X_OFFSET=30;

protected:
	QMainWindow *_qmainWindow;
	MariaTaskManager *_taskManager;

private:
	std::vector<MariaUITask*> _taskStack;
	std::vector<MariaUITask*> _taskDisposeStack;

	float _taskStartHeight;
	int _page;

public:
	MariaUIStateDisplay(QMainWindow* qmainWindow, MariaTaskManager *taskManager, float taskStartHeight);
	~MariaUIStateDisplay();

	MariaUITask* addTask(MariaTask *task, MariaUITask::DISPLAY_TYPE type);
	void updateTask();
	void updateTaskDestination(QPointF destination);
	void updateTaskPosition(QPointF position);
	void updateNumber();
	MariaUITask* eraseTask(int index);
	MariaUITask* eraseTask(MariaTask* task);
	void eraseAllTask();
	void clearTask();
	int getTotalTask();

	void setPage(int page);
	int getPage();
	void updatePage();
};