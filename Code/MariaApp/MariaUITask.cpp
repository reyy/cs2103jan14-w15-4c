#include <assert.h>
#include "MariaUITask.h"
#include "MariaTime.h"

const float MariaUITask::START_END_TIME_WIDTH = 90.0;
const float MariaUITask::FLOW_FACTOR = 0.1;
const float MariaUITask::VALUE_THRESHOLD = 1.0;
const float MariaUITask::FONT_SIZE_TITLE = 16.0;
const float MariaUITask::FONT_SIZE_TIME = 12.0;
const float MariaUITask::FONT_SIZE_DESCRIPTION = 10.0;
const float MariaUITask::FONT_SIZE_TITLE_CONTRACTED = 16.0;
const float MariaUITask::TASK_HEIGHT = 36.0;
const float MariaUITask::TASK_HEIGHT_FLOATING = 25.0;
const float MariaUITask::TASK_HEIGHT_EXPANDED = 150.0;
const float MariaUITask::TASK_HEIGHT_CONTRACTED = 90.0;
const float MariaUITask::TASK_HEIGHT_CONTRACTED_WIDTH = 250.0;
const float MariaUITask::DESCRIPTION_X_OFFSET = 0.0;
const float MariaUITask::DESCRIPTION_Y_OFFSET = 20.0;
const float MariaUITask::TIME_Y_OFFSET = 6.0;
const string MariaUITask::MESSAGE_DEADLINETASK_DUE = "Due in ";
const string MariaUITask::MESSAGE_DEADLINETASK_OVERDUE = "Overdue";
const string MariaUITask::MESSAGE_TIMEDTASK_BEFORE = "Starting in ";
const string MariaUITask::MESSAGE_TIMEDTASK_AFTER = "Event started";
const string MariaUITask::MESSAGE_TIME_START = "Start: ";
const string MariaUITask::MESSAGE_TIME_END = "End: ";
const string MariaUITask::MESSAGE_TIME_DUE = "Due: ";
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
	_startEndText = NULL;
	_completed = NULL;

	_active = false;

	_updatePositionTimer = NULL;
	_updateTimeTextTimer = NULL;
}

MariaUITask::~MariaUITask() {
	deactivate();
}

void MariaUITask::setDisplayTitle() {

	_displayTitle = new QLabel(_qmainWindow);
	_displayTitle->setAlignment(Qt::AlignLeft);
	_displayTitle->setGeometry(QRect(-_qmainWindow->width(),0,0,0));
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

	_typeOfTask = new QLabel(_qmainWindow);
	_typeOfTask->setPixmap(*(MariaUI::getImageHandler(_taskType)));
	_typeOfTask->setGeometry(QRect(-_qmainWindow->width(),0,0,0));
	_typeOfTask->hide();
}

void MariaUITask::setTimeTitle() {
	_timeText = new QLabel(_qmainWindow);
	_timeText->setAlignment(Qt::AlignRight);
	_timeText->setStyleSheet("color:#000000;font-size:" + QString::number(FONT_SIZE_TIME) + "px; padding-right: " + QString::number(TEXTBOX_X_OFFSET) + "px;");
	_timeText->setGeometry(QRect(-_qmainWindow->width(),0,0,0));
	_timeText->hide();
}

void MariaUITask::setDescription() {
	_desciptionText = new QLabel(_qmainWindow);
	_desciptionText->setStyleSheet("color:#000000;font-size:" + QString::number(FONT_SIZE_DESCRIPTION) + "px; padding-right: " + QString::number(TEXTBOX_X_OFFSET) + "px;");
	_desciptionText->setAlignment(Qt::AlignLeft);
	_desciptionText->setWordWrap(true);
	_desciptionText->setGeometry(QRect(-_qmainWindow->width(),0,0,0));
	_desciptionText->hide();
}

void MariaUITask::setTimeAndIcon() {
	_startEndText = new QLabel(_qmainWindow);
	_startEndText->setStyleSheet("color:#000000;font-size:" + QString::number(FONT_SIZE_DESCRIPTION) + "px;");
	_startEndText->setAlignment(Qt::AlignRight);
	_startEndText->setGeometry(QRect(-_qmainWindow->width(),0,0,0));
	_startEndText->hide();

	_completed = new QLabel(_qmainWindow);
	_completed->setAlignment(Qt::AlignCenter);
	_completed->setStyleSheet("color:#000000;font-size:" + QString::number(FONT_SIZE_DESCRIPTION) + "px;");
	_completed->setGeometry(QRect(-_qmainWindow->width(),0,0,0));
	_completed->setPixmap(*MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED));
	_completed->hide();
}

bool MariaUITask::updatePosition() {
	if(_active) {
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
					_desciptionText->setGeometry(QRect(_position.x() + TEXTBOX_X_OFFSET + DESCRIPTION_X_OFFSET, _position.y() + DESCRIPTION_Y_OFFSET, TEXTBOX_X_OFFSET + DESCRIPTION_X_OFFSET + TASK_HEIGHT_CONTRACTED_WIDTH , TASK_HEIGHT_CONTRACTED - DESCRIPTION_Y_OFFSET*2));
					_startEndText->setGeometry(QRect(_position.x() + _width - TEXTBOX_X_OFFSET - START_END_TIME_WIDTH - (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->width(), _position.y() + DESCRIPTION_Y_OFFSET*0.5, START_END_TIME_WIDTH, TASK_HEIGHT_CONTRACTED - DESCRIPTION_Y_OFFSET));
					_completed->setGeometry(QRect(_position.x() + _width - TEXTBOX_X_OFFSET - (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->width(),  _position.y() + DESCRIPTION_Y_OFFSET*0.5, (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->width(), (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->height()));
					break;
				}
				_typeOfTask->setGeometry(QRect(_position.x() + TEXTBOX_X_OFFSET + BULLET_X_OFFSET, _position.y() + BULLET_Y_OFFSET, (MariaUI::getImageHandler(_taskType))->width(), (MariaUI::getImageHandler(_taskType))->height()));
				return true;
		} else {
			if(_updatePositionTimer->isActive()) {
				_updatePositionTimer->stop();
			}
			//Check if it has the chance of deactivating itself.
			if(_position.y() < 0.0 || _position.y() > _qmainWindow->height()) {
				deactivate();
			}
			return false;
		}
	}

	return false;
}

void MariaUITask::updateTimeText() {
	if(_active && _taskReference != NULL&&_currentType == NORMAL) {
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
	if(_taskReference != NULL && _displayTitle != NULL) {
		_displayTitle->setText(QString::fromStdString(pretext) + QString::fromStdString(_taskReference->getTitle()));
		return true;
	} else {
		return false;
	}
}

void MariaUITask::setPosition(QPointF position) {
	_position = position;
	if(_updatePositionTimer!=NULL&&!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(1);
	}
}

QPointF MariaUITask::getPosition() {
	return _position;
}

void MariaUITask::setDestination(QPointF destination) {
	_destination = destination;
	if(_updatePositionTimer!=NULL&&!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(1);
	}
}

QPointF MariaUITask::getDestination() {
	return _destination;
}

bool MariaUITask::updateDetails() {
	if(_active && _taskReference != NULL) {
		_taskType = _taskReference->getType();

		_typeOfTask->setPixmap(*(MariaUI::getImageHandler(_taskType)));
		if(_displayTitle!= NULL) {
			string tempText = _taskReference->getTitle();
			if(_currentType == CONTRACTED) {
				if(tempText.size() > TITLE_CHAR_LIMIT_CONTRACTED) {
					tempText = tempText.substr(0,TITLE_CHAR_LIMIT_CONTRACTED) + "...";
				}
			} else {
				if(tempText.size() > TITLE_CHAR_LIMIT) {
					tempText = tempText.substr(0,TITLE_CHAR_LIMIT) + "...";
				}
			}
			_displayTitle->setText(QString::fromStdString(tempText));
		}
		if(_desciptionText!= NULL) {
			string tempText = _taskReference->getDescription();
			if(tempText.size() > DESCRIPTION_CHAR_LIMIT) {
				tempText = tempText.substr(0,DESCRIPTION_CHAR_LIMIT) + "...";
			}
			_desciptionText->setText(QString::fromStdString(tempText));
		}
		if(_startEndText!= NULL) {
			string timerText;
			MariaTask* temp = _taskReference;
			switch(temp->getType()) {
			case MariaTask::TaskType::DEADLINE:
				timerText += MESSAGE_TIME_DUE;
				timerText += MariaTime::convertToDateString(_taskReference->getEnd()) + "\n";
				timerText += MariaTime::convertToTimeString(_taskReference->getEnd());
				break;
			case MariaTask::TaskType::TIMED:
				timerText += MESSAGE_TIME_START;
				timerText += MariaTime::convertToDateString(_taskReference->getStart()) + "\n";
				timerText += MariaTime::convertToTimeString(_taskReference->getStart()) + "\n";
				timerText += MESSAGE_TIME_END;
				timerText += MariaTime::convertToDateString(_taskReference->getEnd()) + "\n";
				timerText += MariaTime::convertToTimeString(_taskReference->getEnd());
				break;
			case MariaTask::TaskType::FLOATING:
			default:
				break;
			}
			_startEndText->setText(QString::fromStdString(timerText));
		}
		if(_completed!= NULL) {
			if(_taskReference->getIsDone()) {
				_completed->setPixmap(*MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED));
			} else {
				_completed->setPixmap(*MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_NOT_COMPLETED));
			}
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
	if(_startEndText != NULL) {
		_startEndText->show();
	}
	if(_completed != NULL) {
		_completed->show();
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
	if(_startEndText != NULL) {
		_startEndText->hide();
	}
	if(_completed != NULL) {
		_completed->hide();
	}
}


float MariaUITask::getTaskHeight() {
	float toReturn = 0.0;

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

void MariaUITask::activate() {
	if(!_active) {
		_active = true;

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
			this->setDescription();
			this->setTimeAndIcon();
			break;
		}

		_updatePositionTimer = new QTimer(this);
		connect(_updatePositionTimer, SIGNAL(timeout()), this, SLOT(updatePosition()));
		_updatePositionTimer->start(1);

		_updateTimeTextTimer = new QTimer(this);
		connect(_updateTimeTextTimer, SIGNAL(timeout()), this, SLOT(updateTimeText()));
		show();

		startUpdatingTime();	
		updateTimeText();
		updateDetails();


	}
}

void MariaUITask::deactivate() {
	if(_active) {
		_active = false;
		if(_updateTimeTextTimer->isActive()) {
			_updateTimeTextTimer->stop();
		}

		delete _updateTimeTextTimer;
		_updateTimeTextTimer = NULL;

		if(_updatePositionTimer->isActive()) {
			_updatePositionTimer->stop();
		}

		delete _updatePositionTimer;
		_updatePositionTimer = NULL;

		if(_completed!= NULL) {
			delete _completed;
		}
		_completed = NULL;

		if(_startEndText!= NULL) {
			delete _startEndText;
		}
		_startEndText = NULL;

		if(_desciptionText!= NULL) {
			delete _desciptionText;
		}
		_desciptionText = NULL;

		if(_timeText!= NULL) {
			delete _timeText;
		}
		_timeText = NULL;

		if(_displayTitle!= NULL) {
			delete _displayTitle;
		}
		_displayTitle = NULL;

		if(_typeOfTask != NULL) {
			delete _typeOfTask;
		}
		_typeOfTask = NULL;
	}
}

bool MariaUITask::isActivated() {
	return _active;
}

MariaTask * MariaUITask::getMariaTask() {
	return _taskReference;
}