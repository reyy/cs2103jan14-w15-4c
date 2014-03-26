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
	static const int TEXTBOX_X_OFFSET = 30;

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

	MariaUITask* addUITask(MariaTask *task, MariaUITask::DISPLAY_TYPE type);
	void updateUITask();
	void updateUITaskDestination(QPointF destination);
	void updateUITaskPosition(QPointF position);
	void updateUITaskNumber();
	MariaUITask* eraseUITask(int index);
	MariaUITask* eraseUITask(MariaTask* task);
	void eraseAllUITask();
	void clearUITask();
	int getTotalUITask();

	void setPage(int page);
	int getPage();
	void updatePage();

	//Inheritable function to update GUI if there is any UITask operations.
	virtual void updateGUI();
};