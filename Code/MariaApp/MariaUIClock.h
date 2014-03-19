#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QAction>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>

class MariaUIClock : QWidget {
	Q_OBJECT

private:
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

	QMainWindow *_qmainWindow;
	
	QLabel *_currentTime;
	QLabel *_currentDate;
	QLabel *_currentDay;
	QTimer *_clockTimer;

protected slots:
	void updateClock();

public:
	MariaUIClock(QMainWindow *qmainWindow);
	~MariaUIClock();

	void startUpdating();
	void updateGUI(QPointF statePosition);
};
