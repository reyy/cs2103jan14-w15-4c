//@author A0111784H
#include <assert.h>
#include "MariaMacros.h"
#include "MariaUITask.h"
#include "MariaTime.h"

const float MariaUITask::START_END_TIME_WIDTH = 120.0;
const float MariaUITask::FLOW_FACTOR = 0.1;
const float MariaUITask::VALUE_THRESHOLD = 1.0;
const float MariaUITask::FONT_SIZE_TITLE = 16.0;
const float MariaUITask::FONT_SIZE_TIME = 12.0;
const float MariaUITask::FONT_SIZE_DESCRIPTION = 10.0;
const float MariaUITask::FONT_SIZE_TITLE_DETAILED = 16.0;
const float MariaUITask::TASK_HEIGHT = 36.0;
const float MariaUITask::TASK_HEIGHT_FLOATING = 25.0;
const float MariaUITask::TASK_HEIGHT_EXPANDED = 150.0;
const float MariaUITask::TASK_HEIGHT_DETAILED = 90.0;
const float MariaUITask::TASK_WIDTH_SHORTEN = 230.0;
const float MariaUITask::DESCRIPTION_X_OFFSET = 0.0;
const float MariaUITask::DESCRIPTION_Y_OFFSET = 20.0;
const float MariaUITask::TIMESTAMP_X_OFFSET = 3.0;
const float MariaUITask::TIME_Y_OFFSET = 6.0;
const string MariaUITask::MESSAGE_DEADLINETASK_DUE = "Due in ";
const string MariaUITask::MESSAGE_DEADLINETASK_OVERDUE = "Overdue";
const string MariaUITask::MESSAGE_TIMEDTASK_BEFORE = "Starting in ";
const string MariaUITask::MESSAGE_TIMEDTASK_AFTER = "Event started";
const string MariaUITask::MESSAGE_TIME_START = "Start: ";
const string MariaUITask::MESSAGE_TIME_END = "End: ";
const string MariaUITask::MESSAGE_TIME_DUE = "Due: ";

MariaUITask::MariaUITask(QMainWindow *qmainWindow, MariaTask *task, DisplayType type) {
	assert(qmainWindow != NULL);
	assert(task!=NULL);

	_qmainWindow = qmainWindow;
	_taskReference = task;
	_currentDisplayType = type;
	_taskType = _taskReference->getType();

	_destination = QPoint(0, 0);
	_position = QPoint(0, 0);

	_displayTitle = NULL;
	_timeText = NULL;
	_desciptionText = NULL;
	_typeOfTask = NULL;
	_startEndText = NULL;
	_completed = NULL;

	_active = false;
	_atLocation = false;

	_updatePositionTimer = NULL;
	_updateTimeTextTimer = NULL;
}

MariaUITask::~MariaUITask() {
	deactivate();
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
	_atLocation = false;
	if(_updatePositionTimer!=NULL&&!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(GUI_UPDATE_FREQUENCY);
	}
}

QPointF MariaUITask::getPosition() {
	return _position;
}

void MariaUITask::setDestination(QPointF destination) {
	_destination = destination;
	_atLocation = false;
	if(_updatePositionTimer!=NULL&&!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(GUI_UPDATE_FREQUENCY);
	}
}

QPointF MariaUITask::getDestination() {
	return _destination;
}

bool MariaUITask::isAtLocation() {
	return _atLocation;
}

bool MariaUITask::updateDetails() {
	if(_active && _taskReference != NULL) {
		_taskType = _taskReference->getType();

		_typeOfTask->setPixmap(*(MariaUI::getImageHandler(_taskType)));

		if(_displayTitle!= NULL) {
			string tempText = _taskReference->getTitle();
			if(_currentDisplayType == DETAILED) {
				if(tempText.size() > TITLE_CHAR_LIMIT_DETAILED) {
					tempText = tempText.substr(0,TITLE_CHAR_LIMIT_DETAILED) + "...";
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

	switch(_currentDisplayType) {
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
	case DETAILED:
		toReturn = TASK_HEIGHT_DETAILED;
		break;
	}
	return toReturn;
}

void MariaUITask::activate() {
	if(!_active) {
		_active = true;

		switch(_currentDisplayType) {
		case NORMAL:
			this->setDisplayTitle();
			this->setTimeTitle();
			break;
		case EXPANDED:
			this->setDisplayTitle();
			this->setDescription();
			this->setTimeAndIcon();
			break;
		case DETAILED:
			this->setDisplayTitle();
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

		SAFE_DELETE(_updateTimeTextTimer);

		if(_updatePositionTimer->isActive()) {
			_updatePositionTimer->stop();
		}

		SAFE_DELETE(_updatePositionTimer);

		if(_completed!= NULL) {
			SAFE_DELETE(_completed);
		}

		if(_startEndText!= NULL) {
			SAFE_DELETE(_startEndText);
		}

		if(_desciptionText!= NULL) {
			SAFE_DELETE(_desciptionText);
		}

		if(_timeText!= NULL) {
			SAFE_DELETE(_timeText);
		}

		if(_displayTitle!= NULL) {
			SAFE_DELETE(_displayTitle);
		}

		if(_typeOfTask != NULL) {
			SAFE_DELETE(_typeOfTask);
		}
	}
}

bool MariaUITask::isActivated() {
	return _active;
}

MariaTask * MariaUITask::getMariaTask() {
	return _taskReference;
}

//@author generated
bool MariaUITask::updatePosition() {
	if(_active) {
		if(abs(_position.x()-_destination.x())>VALUE_THRESHOLD||
			abs(_position.y()-_destination.y())>VALUE_THRESHOLD) {
				_position.setX(_position.x() + (_destination.x()-_position.x())*FLOW_FACTOR);
				_position.setY(_position.y() + (_destination.y()-_position.y())*FLOW_FACTOR);
				switch(_currentDisplayType) {
				case NORMAL:
					if(_taskType == MariaTask::FLOATING) {
						_displayTitle->setGeometry(_position.x(), _position.y(), _qmainWindow->width(), TASK_HEIGHT_FLOATING);
					} else {
						_displayTitle->setGeometry(_position.x(), _position.y(), _qmainWindow->width(), TASK_HEIGHT);
					}
					_timeText->setGeometry(_position.x() + TIMESTAMP_X_OFFSET, _position.y()+TASK_HEIGHT-TIME_Y_OFFSET-FONT_SIZE_TIME, _qmainWindow->width(), TASK_HEIGHT);
					break;
				case EXPANDED:
					_displayTitle->setGeometry(_position.x(), _position.y(), _qmainWindow->width(), TASK_HEIGHT_EXPANDED);
					_desciptionText->setGeometry(_position.x() + TEXTBOX_X_OFFSET + DESCRIPTION_X_OFFSET, _position.y() + DESCRIPTION_Y_OFFSET, TEXTBOX_X_OFFSET + DESCRIPTION_X_OFFSET + TASK_WIDTH_SHORTEN , TASK_HEIGHT_EXPANDED - DESCRIPTION_Y_OFFSET*2);
					_startEndText->setGeometry(_position.x() + _qmainWindow->width() - TEXTBOX_X_OFFSET - START_END_TIME_WIDTH - (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->width(), _position.y() + DESCRIPTION_Y_OFFSET*0.5, START_END_TIME_WIDTH, TASK_HEIGHT_EXPANDED - DESCRIPTION_Y_OFFSET);
					_completed->setGeometry(_position.x() + _qmainWindow->width() - TEXTBOX_X_OFFSET - (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->width(),  _position.y() + DESCRIPTION_Y_OFFSET*0.5, (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->width(), (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->height());
					break;
				case DETAILED:
					_displayTitle->setGeometry(_position.x(), _position.y(), _qmainWindow->width(), TASK_HEIGHT_DETAILED);
					_desciptionText->setGeometry(_position.x() + TEXTBOX_X_OFFSET + DESCRIPTION_X_OFFSET, _position.y() + DESCRIPTION_Y_OFFSET, TEXTBOX_X_OFFSET + DESCRIPTION_X_OFFSET + TASK_WIDTH_SHORTEN , TASK_HEIGHT_DETAILED - DESCRIPTION_Y_OFFSET*2);
					_startEndText->setGeometry(_position.x() + _qmainWindow->width() - TEXTBOX_X_OFFSET - START_END_TIME_WIDTH - (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->width(), _position.y() + DESCRIPTION_Y_OFFSET*0.5, START_END_TIME_WIDTH, TASK_HEIGHT_DETAILED - DESCRIPTION_Y_OFFSET);
					_completed->setGeometry(_position.x() + _qmainWindow->width() - TEXTBOX_X_OFFSET - (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->width(),  _position.y() + DESCRIPTION_Y_OFFSET*0.5, (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->width(), (MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED))->height());
					break;
				}
				_typeOfTask->setGeometry(_position.x() + TEXTBOX_X_OFFSET + BULLET_X_OFFSET, _position.y() + BULLET_Y_OFFSET, (MariaUI::getImageHandler(_taskType))->width(), (MariaUI::getImageHandler(_taskType))->height());
				return true;
		} else {
			if(_updatePositionTimer->isActive()) {
				_updatePositionTimer->stop();
			}
			_atLocation = true;
			//Check if it has the chance of deactivating itself.
			if(_position.y() < 0.0 || _position.y() > _qmainWindow->height()) {
				deactivate();
			}
			return false;
		}
	}

	return false;
}

//@author A0111784H
void MariaUITask::updateTimeText() {
	if(_active && _taskReference != NULL&&_currentDisplayType == NORMAL) {
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

//@author generated
void MariaUITask::setDisplayTitle() {

	_displayTitle = new QLabel(_qmainWindow);
	_displayTitle->setAlignment(Qt::AlignLeft);
	_displayTitle->setGeometry(-_qmainWindow->width(),0,0,0);
	_displayTitle->hide();

	switch(_currentDisplayType) {

	case EXPANDED:
		_displayTitle->setStyleSheet("color:#000000; padding-left: " + QString::number(BULLET_SPACE + TEXTBOX_X_OFFSET) + "px; background-color:rgba(255, 255, 255, 255);border: 1px solid rgba(200, 200, 200, 255);font-size:" + QString::number(FONT_SIZE_TITLE) + "px;");
		break;
	case DETAILED:
		_displayTitle->setStyleSheet("color:#000000; padding-left: " + QString::number(BULLET_SPACE + TEXTBOX_X_OFFSET) + "px; background-color:rgba(255, 255, 255, 255);border: 1px solid rgba(200, 200, 200, 255);font-size:" + QString::number(FONT_SIZE_TITLE_DETAILED) + "px;");
		break;
	case NORMAL:
	default:
		_displayTitle->setStyleSheet("color:#000000; padding-left: " + QString::number(BULLET_SPACE + TEXTBOX_X_OFFSET) + "px; background-color:rgba(255, 255, 255, 255);border: 1px solid rgba(200, 200, 200, 255);font-size:" + QString::number(FONT_SIZE_TITLE) + "px;");
		break;
	}

	_typeOfTask = new QLabel(_qmainWindow);
	_typeOfTask->setPixmap(*(MariaUI::getImageHandler(_taskType)));
	_typeOfTask->setGeometry(-_qmainWindow->width(),0,0,0);
	_typeOfTask->hide();
}

void MariaUITask::setTimeTitle() {
	_timeText = new QLabel(_qmainWindow);
	_timeText->setAlignment(Qt::AlignRight);
	_timeText->setStyleSheet("color:#000000;font-size:" + QString::number(FONT_SIZE_TIME) + "px; padding-right: " + QString::number(TEXTBOX_X_OFFSET) + "px;");
	_timeText->setGeometry(-_qmainWindow->width(),0,0,0);
	_timeText->hide();
}

void MariaUITask::setDescription() {
	_desciptionText = new QLabel(_qmainWindow);
	_desciptionText->setStyleSheet("color:#000000;font-size:" + QString::number(FONT_SIZE_DESCRIPTION) + "px; padding-right: " + QString::number(TEXTBOX_X_OFFSET) + "px;");
	_desciptionText->setAlignment(Qt::AlignLeft);
	_desciptionText->setWordWrap(true);
	_desciptionText->setGeometry(-_qmainWindow->width(),0,0,0);
	_desciptionText->hide();
}

void MariaUITask::setTimeAndIcon() {
	_startEndText = new QLabel(_qmainWindow);
	_startEndText->setStyleSheet("color:#000000;font-size:" + QString::number(FONT_SIZE_DESCRIPTION) + "px;");
	_startEndText->setAlignment(Qt::AlignRight);
	_startEndText->setGeometry(-_qmainWindow->width(),0,0,0);
	_startEndText->hide();

	_completed = new QLabel(_qmainWindow);
	_completed->setAlignment(Qt::AlignCenter);
	_completed->setStyleSheet("color:#000000;font-size:" + QString::number(FONT_SIZE_DESCRIPTION) + "px;");
	_completed->setGeometry(-_qmainWindow->width(),0,0,0);
	_completed->setPixmap(*MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_COMPLETED));
	_completed->hide();
}