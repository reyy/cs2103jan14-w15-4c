#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>
#include <string>
#include "MariaTask.h"

class MariaUITask : QWidget{
	Q_OBJECT
public:
	static const int TEXTBOX_X_OFFSET=20;
	static const int BULLET_SPACE=10;
	static const int TIME_UPDATE_FREQUENCY=1000;
	static const int BULLET_X_OFFSET=2;
	static const int BULLET_Y_OFFSET=4;
	static const float FLOW_FACTOR;
	static const float VALUE_THRESHOLD;
	static const float FONT_SIZE;
	static const float TASK_HEIGHT;
	static const float TIMESTAMP_X_OFFSET;
	static const string MESSAGE_DEADLINETASK_DUE;
	static const string MESSAGE_DEADLINETASK_OVERDUE;
	static const string MESSAGE_TIMEDTASK_BEFORE;
	static const string MESSAGE_TIMEDTASK_AFTER;

private:
	QMainWindow * _qmainWindow;
	QPointF _destination;
	QPointF _position;
	
	QLabel *_displayTitle;
	MariaTask *_taskReference;
	QLabel *_timeText;
	QLabel *_typeOfTask;
	QPixmap *_taskTypeIconHandler;
	float _width;

	
	QTimer *_updatePositionTimer;
	QTimer *_updateTimeTextTimer;

protected slots:
	bool updatePosition();
	void updateTimeText();

public:
	MariaUITask(QMainWindow *qmainWindow, MariaTask *task, float width);
	~MariaUITask();

	bool setTitlePretext(string pretext);
	void setPosition(QPointF position);
	QPointF getPosition();
	void setDestination(QPointF destination);
	QPointF getDestination();

	bool updateDetails();

	void stopUpdatingTime();
	void startUpdatingTime();

	void show();
	void hide();

	MariaTask * getMariaTask();
};
