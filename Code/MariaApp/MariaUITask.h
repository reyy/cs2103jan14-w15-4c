//@author A0111784H
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
	static const int GUI_UPDATE_FREQUENCY = 1;
	static const int TIME_UPDATE_FREQUENCY = 1000;
	static const int TITLE_CHAR_LIMIT = 40;
	static const int TITLE_CHAR_LIMIT_DETAILED = 25;
	static const int DESCRIPTION_CHAR_LIMIT = 120;
	static const int TEXTBOX_X_OFFSET = 30;
	static const int BULLET_SPACE = 20;
	static const int BULLET_X_OFFSET = 2;
	static const int BULLET_Y_OFFSET = 4;
	static const float START_END_TIME_WIDTH;
	static const float FLOW_FACTOR;
	static const float VALUE_THRESHOLD;
	static const float FONT_SIZE_TITLE;
	static const float FONT_SIZE_TIME;
	static const float FONT_SIZE_DESCRIPTION;
	static const float FONT_SIZE_TITLE_DETAILED;
	static const float TASK_HEIGHT;
	static const float TASK_HEIGHT_FLOATING;
	static const float TASK_HEIGHT_EXPANDED;
	static const float TASK_HEIGHT_DETAILED;
	static const float TASK_WIDTH_SHORTEN;
	static const float DESCRIPTION_X_OFFSET;
	static const float DESCRIPTION_Y_OFFSET;
	static const float TIMESTAMP_X_OFFSET;
	static const float TIME_Y_OFFSET;
	static const string MESSAGE_DEADLINETASK_DUE;
	static const string MESSAGE_DEADLINETASK_OVERDUE;
	static const string MESSAGE_TIMEDTASK_BEFORE;
	static const string MESSAGE_TIMEDTASK_AFTER;
	static const string MESSAGE_TIME_START;
	static const string MESSAGE_TIME_END;
	static const string MESSAGE_TIME_DUE;

	typedef enum{
		NORMAL, 
		EXPANDED, 
		DETAILED
	} DisplayType;

	MariaUITask(QMainWindow* qmainWindow, MariaTask* task, DisplayType type);
	~MariaUITask();

	bool setTitlePretext(string pretext);
	void setPosition(QPointF position);
	QPointF getPosition();
	void setDestination(QPointF destination);
	QPointF getDestination();
	bool isAtLocation();
	bool updateDetails();

	void stopUpdatingTime();
	void startUpdatingTime();

	void show();
	void hide();

	float getTaskHeight();

	void activate();
	void deactivate();
	bool isActivated();

	MariaTask* getMariaTask();

protected slots:
	bool updatePosition();
	void updateTimeText();

private:
	QMainWindow* _qmainWindow;
	MariaTask* _taskReference;
	DisplayType _currentDisplayType;
	MariaTask::TaskType _taskType;
	
	QPointF _destination;
	QPointF _position;

	QLabel* _displayTitle;
	QLabel* _timeText;
	QLabel* _desciptionText;
	QLabel* _typeOfTask;
	QLabel* _startEndText;
	QLabel* _completed;

	bool _active;
	bool _atLocation;
	
	QTimer* _updatePositionTimer;
	QTimer* _updateTimeTextTimer;

	void setDisplayTitle();
	void setTimeTitle();
	void setDescription();
	void setTimeAndIcon();
};
