#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"
#include "MariaUIClock.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaUIStateHome : public MariaStateObject {
public:
	typedef enum {
		FLOATING,
		DEADLINE,
		TIMED
	} TYPE_OF_TASK;
private:
	QMainWindow *_qmainWindow;

	MariaUIClock *_clock;
	std::vector<MariaUITask*> _floatingStack;
	std::vector<MariaUITask*> _deadlineStack;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();
public:
	MariaUIStateHome(QMainWindow* qmainWindow);
	~MariaUIStateHome();

	void addTask(MariaTask task);
	void eraseTask(TYPE_OF_TASK,int index);
	void clearTask(TYPE_OF_TASK);
};

