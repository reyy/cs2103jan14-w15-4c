#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"

class MariaUIStateShow : public MariaStateObject {
	
	QMainWindow *_qmainWindow;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();
public:
	MariaUIStateShow(QMainWindow* qmainWindow);
	~MariaUIStateShow();
};

