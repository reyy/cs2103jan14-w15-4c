#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"
#include "MariaUIClock.h"
#include "MariaUIDisplayPack.h"

class MariaUIStateHome : public MariaStateObject {
	
	QMainWindow *_qmainWindow;

	MariaUIClock *_clock;
	std::vector<MariaUIDisplayPack*> _displayPackStack;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();
public:
	MariaUIStateHome(QMainWindow* qmainWindow);
	~MariaUIStateHome();
};

