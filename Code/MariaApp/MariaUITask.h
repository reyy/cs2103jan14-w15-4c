#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>
#include <string>
#include "MariaUI.h"
#include "MariaTask.h"

class MariaUITask : QWidget{
	Q_OBJECT
public:
	static const int TEXTBOX_X_OFFSET = 30;
	static const int BULLET_SPACE = 20;
	static const int TIME_UPDATE_FREQUENCY = 1000;
	static const int BULLET_X_OFFSET = 2;
	static const int BULLET_Y_OFFSET = 4;
	static const float FLOW_FACTOR;
	static const float VALUE_THRESHOLD;
	static const float FONT_SIZE_TITLE;
	static const float FONT_SIZE_TIME;
	static const float FONT_SIZE_DESCRIPTION;
	static const float FONT_SIZE_TITLE_CONTRACTED;
	static const float TASK_HEIGHT;
	static const float TASK_HEIGHT_FLOATING;
	static const float TASK_HEIGHT_EXPANDED;
	static const float TASK_HEIGHT_CONTRACTED;
	static const float DESCRIPTION_X_OFFSET;
	static const float DESCRIPTION_Y_OFFSET;
	static const float TIME_Y_OFFSET;
	static const string MESSAGE_DEADLINETASK_DUE;
	static const string MESSAGE_DEADLINETASK_OVERDUE;
	static const string MESSAGE_TIMEDTASK_BEFORE;
	static const string MESSAGE_TIMEDTASK_AFTER;
	static const float TIMESTAMP_X_OFFSET;

	typedef enum{
		NORMAL, 
		EXPANDED,
		CONTRACTED
	} DISPLAY_TYPE;

private:
	QMainWindow * _qmainWindow;
	QPointF _destination;
	QPointF _position;
	
	DISPLAY_TYPE _currentType;
	QLabel *_displayTitle;
	MariaTask *_taskReference;
	QLabel *_desciptionText;
	QLabel *_timeText;
	QLabel *_typeOfTask;
	float _width;
	MariaTask::TaskType _taskType;
	
	QTimer *_updatePositionTimer;
	QTimer *_updateTimeTextTimer;

	void setDisplayTitle();
	void setTimeTitle();
	void setDescription();

protected slots:
	bool updatePosition();
	void updateTimeText();

public:
	MariaUITask(QMainWindow *qmainWindow, MariaTask *task, float width, DISPLAY_TYPE type);
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

	float getTaskHeight();

	MariaTask * getMariaTask();
};
