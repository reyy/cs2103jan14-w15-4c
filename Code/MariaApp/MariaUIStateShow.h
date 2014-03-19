#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"
#include "MariaTime.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaTaskManager;
class MariaUIStateShow : public MariaStateObject {
public:
	static const int TEXTBOX_X_OFFSET=30;
	static const float TASKBAR_STARTHEIGHT_SCALE;
	static const float TASK_STARTHEIGHT_SCALE;
	static const float TASK_STARTHEIGHT_DIFFERENCE;
	static const int MAX_ITEM_IN_PAGE=20;

	typedef enum {
		DAY,
		MONTH,
		YEAR
	} VIEW_TYPE;

private:
	QMainWindow *_qmainWindow;
	MariaTaskManager* _taskManager;

	QPixmap *_currentTimeLineImage;
	QPixmap *_timeLineImage;
	
	QLabel *_currentTimeLine;


	QLabel *_titleLabel;

	QLabel *_monthLabel[12];


	VIEW_TYPE _currentViewType;
	MariaTime _targetTime;
	
	int _page;

	std::vector<MariaUITask*> _taskStack;
	std::vector<MariaUITask*> _taskDisposeStack;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();

	static bool sortTaskFunction(MariaUITask *i,MariaUITask *j);
public:
	MariaUIStateShow(QMainWindow* qmainWindow,MariaTaskManager *taskManager,VIEW_TYPE currentViewType,MariaTime targetTime);
	~MariaUIStateShow();

	MariaUITask* addTask(MariaTask *task);
	void updateTask();
	MariaUITask* eraseTask(int index);
	MariaUITask* eraseTask(MariaTask* task);
	void eraseAllTask();
	void clearTask();
	void sortTask();
	int getTotalTask();

	void setPage(int page);
	int getPage();
	void updatePage();
};

