//@author A0111784H 
#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>

class MariaUIClock : QWidget {
	Q_OBJECT

public:
	MariaUIClock(QMainWindow *qmainWindow);
	~MariaUIClock();

	void startUpdating();
	void updateGUI(QPointF statePosition);

protected slots:
	void updateClock();

private:
	static const int CLOCK_UPDATE_FREQUENCY = 1000;
	static const int DAYS_OFFSET = 6;
	static const int DAYS_IN_WEEK = 7;
	static const float START_HEIGHT_SCALE;
	static const float TIME_WIDTH;
	static const float TIME_HEIGHT;
	static const float TIME_X_OFFSET;
	static const float TIME_Y_OFFSET;
	static const float DATE_WIDTH;
	static const float DATE_HEIGHT;
	static const float DATE_X_OFFSET;
	static const float DATE_Y_OFFSET;
	static const float DAY_WIDTH;
	static const float DAY_HEIGHT;
	static const float DAY_X_OFFSET;
	static const float DAY_Y_OFFSET;
	static const float LINE_WIDTH;
	static const float LINE_HEIGHT;
	static const float LINE_X_OFFSET;
	static const float LINE_Y_OFFSET;

	QMainWindow *_qmainWindow;
	
	QLabel *_currentTime;
	QLabel *_currentDate;
	QLabel *_currentDay;
	QTimer *_clockTimer;
	QLabel *_line;
};
