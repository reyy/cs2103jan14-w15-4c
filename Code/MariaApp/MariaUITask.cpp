#include <assert.h>
#include "MariaUITask.h"
#include "MariaTime.h"

const float MariaUITask::FLOW_FACTOR = 0.05;
const float MariaUITask::VALUE_THRESHOLD = 1.0;
const float MariaUITask::FONT_SIZE_TITLE = 16.0;
const float MariaUITask::FONT_SIZE_TIME = 12.0;
const float MariaUITask::FONT_SIZE_DESCRIPTION = 12.0;
const float MariaUITask::FONT_SIZE_TITLE_CONTRACTED = 12.0;
const float MariaUITask::TASK_HEIGHT = 36.0;
const float MariaUITask::TASK_HEIGHT_FLOATING = 25.0;
const float MariaUITask::TASK_HEIGHT_EXPANDED = 150.0;
const float MariaUITask::TASK_HEIGHT_CONTRACTED = 16.0;
const float MariaUITask::DESCRIPTION_X_OFFSET = 0.0;
const float MariaUITask::DESCRIPTION_Y_OFFSET = 30.0;
const float MariaUITask::TIME_Y_OFFSET = 6.0;
const string MariaUITask::MESSAGE_DEADLINETASK_DUE = "Due in ";
const string MariaUITask::MESSAGE_DEADLINETASK_OVERDUE = "Overdue";
const string MariaUITask::MESSAGE_TIMEDTASK_BEFORE = "Starting in ";
const string MariaUITask::MESSAGE_TIMEDTASK_AFTER = "Event started";
const float MariaUITask::TIMESTAMP_X_OFFSET = 3.0;

MariaUITask::MariaUITask(QMainWindow *qmainWindow, MariaTask *task, float width, DISPLAY_TYPE type) {
	_qmainWindow = qmainWindow;

	assert(task!=NULL);

	_taskReference = task;
	_currentType = type;
	_width = width;
	_taskType = _taskReference->getType();

	_destination = QPoint(0, 0);
	_position = QPoint(0, 0);

	_displayTitle = NULL;
	_timeText = NULL;
	_desciptionText = NULL;

	switch(_currentType) {
	case NORMAL:
		this->setDisplayTitle();
		this->setTimeTitle();
		break;
	case EXPANDED:
		this->setDisplayTitle();
		this->setTimeTitle();
		this->setDescription();
		break;
	case CONTRACTED:
		this->setDisplayTitle();
		this->setTimeTitle();
		break;
	}

	loadImages();

	_updatePositionTimer = new QTimer(this);
	connect(_updatePositionTimer, SIGNAL(timeout()), this, SLOT(updatePosition()));
	_updatePositionTimer->start(1);

	_updateTimeTextTimer=NULL;
	if(_currentType == NORMAL) {
		_updateTimeTextTimer = new QTimer(this);
		connect(_updateTimeTextTimer, SIGNAL(timeout()), this, SLOT(updateTimeText()));
		updateTimeText();
	}
	updateDetails();
}

MariaUITask::~MariaUITask() {
	if(_updateTimeTextTimer!= NULL) {
		if(_updateTimeTextTimer->isActive()) {
			_updateTimeTextTimer->stop();
		}

		delete _updateTimeTextTimer;
	}

	if(_updatePositionTimer->isActive()) {
		_updatePositionTimer->stop();
	}

	delete _updatePositionTimer;

	if(_desciptionText!= NULL) {
		delete _desciptionText;
	}

	if(_timeText!= NULL) {
		delete _timeText;
	}

	if(_displayTitle!= NULL) {
		delete _displayTitle;
	}

	if(_typeOfTask != NULL) {
		delete _taskTypeIconHandler[2];
		delete _taskTypeIconHandler[1];
		delete _taskTypeIconHandler[0];
		delete _typeOfTask;
	}
}

void MariaUITask::setDisplayTitle() {

	_displayTitle = new QLabel(_qmainWindow);
	_displayTitle->setAlignment(Qt::AlignLeft);
	_displayTitle->hide();

	switch(_currentType) {
	
	case EXPANDED:
		_displayTitle->setStyleSheet("color:#000000; padding-left: " + QString::number(BULLET_SPACE + TEXTBOX_X_OFFSET) + "px; background-color:rgba(255, 255, 255, 255);border: 1px solid rgba(200, 200, 200, 255);font-size:" + QString::number(FONT_SIZE_TITLE) + "px;");
		break;
	case CONTRACTED:
		_displayTitle->setStyleSheet("color:#000000; padding-left: " + QString::number(BULLET_SPACE + TEXTBOX_X_OFFSET) + "px; background-color:rgba(255, 255, 255, 255);border: 1px solid rgba(200, 200, 200, 255);font-size:" + QString::number(FONT_SIZE_TITLE_CONTRACTED) + "px;");
		break;
	case NORMAL:
	default:
		_displayTitle->setStyleSheet("color:#000000; padding-left: " + QString::number(BULLET_SPACE + TEXTBOX_X_OFFSET) + "px; background-color:rgba(255, 255, 255, 255);border: 1px solid rgba(200, 200, 200, 255);font-size:" + QString::number(FONT_SIZE_TITLE) + "px;");
		break;
	}
}

void MariaUITask::setTimeTitle() {
	_timeText = new QLabel(_qmainWindow);
	_timeText->setAlignment(Qt::AlignRight);
	_timeText->setStyleSheet("color:#000000;font-size:" + QString::number(FONT_SIZE_TIME) + "px; padding-right: " + QString::number(TEXTBOX_X_OFFSET) + "px;");
	_timeText->hide();
}

void MariaUITask::setDescription() {
	_desciptionText = new QLabel(_qmainWindow);
	_desciptionText->setStyleSheet("border: 1px solid black; color:#000000;font-size:" + QString::number(FONT_SIZE_DESCRIPTION) + "px; padding-right: " + QString::number(TEXTBOX_X_OFFSET) + "px;");
	_desciptionText->setAlignment(Qt::AlignLeft);
	_desciptionText->setWordWrap(true);
	_desciptionText->hide();
}

void MariaUITask::loadImages() {
	_taskTypeIconHandler[0] = new QPixmap("./Resources/ui_task_type_floating.png");
	_taskTypeIconHandler[1] = new QPixmap("./Resources/ui_task_type_deadline.png");
	_taskTypeIconHandler[2] = new QPixmap("./Resources/ui_task_type_timed.png");
	
	_typeOfTask = new QLabel(_qmainWindow);
	_typeOfTask->setPixmap(*_taskTypeIconHandler[_taskType]);
	_typeOfTask->hide();
}

bool MariaUITask::updatePosition() {
	if(abs(_position.x()-_destination.x())>VALUE_THRESHOLD||
		abs(_position.y()-_destination.y())>VALUE_THRESHOLD) {
			_position.setX(_position.x() + (_destination.x()-_position.x())*FLOW_FACTOR);
			_position.setY(_position.y() + (_destination.y()-_position.y())*FLOW_FACTOR);
			switch(_currentType) {
			case NORMAL:
				if(_taskType == MariaTask::FLOATING) {
					_displayTitle->setGeometry(QRect(_position.x(), _position.y(), _width, TASK_HEIGHT_FLOATING));
				} else {
					_displayTitle->setGeometry(QRect(_position.x(), _position.y(), _width, TASK_HEIGHT));
				}
				_timeText->setGeometry(QRect(_position.x() + TIMESTAMP_X_OFFSET, _position.y()+TASK_HEIGHT-TIME_Y_OFFSET-FONT_SIZE_TIME, _width, TASK_HEIGHT));
				break;
			case EXPANDED:
				_displayTitle->setGeometry(QRect(_position.x(), _position.y(), _width, TASK_HEIGHT_EXPANDED));
				_desciptionText->setGeometry(QRect(_position.x() + TEXTBOX_X_OFFSET + DESCRIPTION_X_OFFSET, _position.y() + DESCRIPTION_Y_OFFSET, _width-(TEXTBOX_X_OFFSET + DESCRIPTION_X_OFFSET)*2, TASK_HEIGHT_EXPANDED-DESCRIPTION_Y_OFFSET*2));
				break;
			case CONTRACTED:
				_displayTitle->setGeometry(QRect(_position.x(), _position.y(), _width, TASK_HEIGHT_CONTRACTED));
				_timeText->setGeometry(QRect(_position.x() + TIMESTAMP_X_OFFSET, _position.y()+TASK_HEIGHT-TIME_Y_OFFSET-FONT_SIZE_TIME, _width, TASK_HEIGHT_CONTRACTED));
				break;
			}
			_typeOfTask->setGeometry(QRect(_position.x() + TEXTBOX_X_OFFSET + BULLET_X_OFFSET, _position.y() + BULLET_Y_OFFSET, _taskTypeIconHandler[_taskType]->width(), _taskTypeIconHandler[_taskType]->height()));
			return true;
	} else {
		if(_updatePositionTimer->isActive()) {
			_updatePositionTimer->stop();
		}
		return false;
	}
}

void MariaUITask::updateTimeText() {
	if(_taskReference != NULL&&_currentType == NORMAL) {
		string timeFormatted = _taskReference->getTimeFromNow();

		if(_taskReference->getType() == MariaTask::DEADLINE) {
			if(timeFormatted.empty()) {
				timeFormatted = MESSAGE_DEADLINETASK_OVERDUE;
			} else {
				timeFormatted = MESSAGE_DEADLINETASK_DUE + timeFormatted;
			}
			_timeText->setText(QString::fromStdString(timeFormatted));

		}else if(_taskReference->getType() == MariaTask::TIMED) {
			if(timeFormatted.empty()) {
				timeFormatted = MESSAGE_TIMEDTASK_AFTER;
			} else {
				timeFormatted = MESSAGE_TIMEDTASK_BEFORE + timeFormatted;
			}
		}
		_timeText->setText(QString::fromStdString(timeFormatted));
	}	
}

bool MariaUITask::setTitlePretext(string pretext) {
	if(_taskReference != NULL) {
		_displayTitle->setText(QString::fromStdString(pretext) + QString::fromStdString(_taskReference->getTitle()));
		return true;
	} else {
		return false;
	}
}

void MariaUITask::setPosition(QPointF position) {
	_position = position;
	if(!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(1);
	}
}

QPointF MariaUITask::getPosition() {
	return _position;
}

void MariaUITask::setDestination(QPointF destination) {
	_destination = destination;
	if(!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(1);
	}
}

QPointF MariaUITask::getDestination() {
	return _destination;
}

bool MariaUITask::updateDetails() {
	if(_taskReference != NULL) {
		_typeOfTask->setPixmap(*_taskTypeIconHandler[_taskType]);
		if(_displayTitle!= NULL) {
			_displayTitle->setText(QString::fromStdString(_taskReference->getTitle()));
		}
		if(_desciptionText!= NULL) {
			_desciptionText->setText(QString::fromStdString(_taskReference->getDescription()));
		}
		return true;
	} else {
		return false;
	}
}

void MariaUITask::stopUpdatingTime() {
	if(_updateTimeTextTimer!= NULL) {
		if(_updateTimeTextTimer->isActive()) {
			_updateTimeTextTimer->stop();
		}
	}
}

void MariaUITask::startUpdatingTime() {
	if(_updateTimeTextTimer!= NULL) {
		if(!_updateTimeTextTimer->isActive()) {
			_updateTimeTextTimer->start(TIME_UPDATE_FREQUENCY);
		}
	}
}

void MariaUITask::show() {
	if(_displayTitle != NULL) {
		_displayTitle->show();
	}
	if(_timeText != NULL) {
		_timeText->show();
	}
	if(_typeOfTask != NULL) {
		_typeOfTask->show();
	}
	if(_desciptionText != NULL) {
		_desciptionText->show();
	}
}

void MariaUITask::hide() {
	if(_displayTitle != NULL) {
		_displayTitle->hide();
	}
	if(_timeText != NULL) {
		_timeText->hide();
	}
	if(_typeOfTask != NULL) {
		_typeOfTask->hide();
	}
	if(_desciptionText != NULL) {
		_desciptionText->hide();
	}
}


float MariaUITask::getTaskHeight() {
	float toReturn;

	switch(_currentType) {
	case NORMAL:
		if(_taskType == MariaTask::FLOATING) {
			toReturn = TASK_HEIGHT_FLOATING;
		} else {
			toReturn = TASK_HEIGHT;
		}
		break;
	case EXPANDED:
		toReturn = TASK_HEIGHT_EXPANDED;
		break;
	case CONTRACTED:
		toReturn = TASK_HEIGHT_CONTRACTED;
		break;
	}
	return toReturn;
}

MariaTask * MariaUITask::getMariaTask() {
	return _taskReference;
}