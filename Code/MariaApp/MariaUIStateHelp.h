#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"

class MariaUIStateHelp : public MariaStateObject {
	
	static const int HELP_IMAGE_WIDTH = 480;
	static const int HELP_IMAGE_HEIGHT = 640;

	QMainWindow *_qmainWindow;
	QLabel *_helpImage;
	int _helpIndex;
	bool _doneViewing;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();
public:
	MariaUIStateHelp(QMainWindow* qmainWindow, int screen = 0);
	~MariaUIStateHelp();

	int getHelpIndex();
	void setHelpIndex(int index);
	void setDoneViewing();
};

