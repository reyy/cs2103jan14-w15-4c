#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>
#include <string>
#include "MariaTask.h"

class MariaUITask : QWidget{
	Q_OBJECT
public:
	static const float FLOW_FACTOR;
	static const float VALUE_THRESHOLD;
	static const float TASK_HEIGHT;
	static const string MESSAGE_DEADLINETASK_DUE;
	static const string MESSAGE_DEADLINETASK_OVERDUE;
	static const string MESSAGE_TIMEDTASK_BEFORE;
	static const string MESSAGE_TIMEDTASK_AFTER;

private:
	QMainWindow * _qmainWindow;
	QPointF _destination;
	QPointF _position;
	
	QLabel *_displayTitle;
	MariaTask::TaskType _taskType;
	MariaTime _deadline;

	QLabel *_timeText;
	float _width;
	
	QTimer *_updatePositionTimer;
	QTimer *_updateTimeTextTimer;

protected slots:
	bool updatePosition();
	void updateTimeText();

public:
	MariaUITask(QMainWindow *qmainWindow, MariaTask task, float width);
	~MariaUITask();

	void setPosition(QPointF position);
	QPointF getPosition();
	void setDestination(QPointF destination);
	QPointF getDestination();

	void show();
	void hide();

};
