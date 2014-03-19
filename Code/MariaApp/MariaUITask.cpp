#include "MariaUITask.h"
#include "MariaTime.h"

const float MariaUITask::FLOW_FACTOR=0.05;
const float MariaUITask::VALUE_THRESHOLD=1.0;
const float MariaUITask::FONT_SIZE_TITLE=16.0;
const float MariaUITask::FONT_SIZE_DESCRIPTION=12.0;
const float MariaUITask::TASK_HEIGHT=24.0;
const float MariaUITask::TASK_HEIGHT_EXPANDED=150.0;
const float MariaUITask::DESCRIPTION_X_OFFSET=0.0;
const float MariaUITask::DESCRIPTION_Y_OFFSET=30.0;
const string MariaUITask::MESSAGE_DEADLINETASK_DUE="Due in ";
const string MariaUITask::MESSAGE_DEADLINETASK_OVERDUE="Overdue";
const string MariaUITask::MESSAGE_TIMEDTASK_BEFORE="Starting in ";
const string MariaUITask::MESSAGE_TIMEDTASK_AFTER="Event started";
const float MariaUITask::TIMESTAMP_X_OFFSET=3.0;

MariaUITask::MariaUITask(QMainWindow *qmainWindow, MariaTask *task, float width,DISPLAY_TYPE type) {
	_qmainWindow=qmainWindow;

	_destination=QPoint(0,0);
	_position=QPoint(0,0);

	_taskReference=task;
	_currentType=type;
	_displayTitle = new QLabel(_qmainWindow);
	_timeText = new QLabel(_qmainWindow);
	_desciptionText = new QLabel(_qmainWindow);
	_typeOfTask = new QLabel(_qmainWindow);
	switch(_taskReference->getType()) {
	case MariaTask::FLOATING:
		_taskTypeIconHandler = new QPixmap("./Resources/ui_task_type_floating.png");
		_displayTitle->setStyleSheet("color:#000000; padding-left: "+QString::number(BULLET_SPACE+TEXTBOX_X_OFFSET)+"px; background-color:rgba(255,255,255,255);border: 1px solid rgba(200,200,200,255);font-size:"+QString::number(FONT_SIZE_TITLE)+"px;");		
		break;
	case MariaTask::DEADLINE:
		_taskTypeIconHandler = new QPixmap("./Resources/ui_task_type_deadline.png");
		_displayTitle->setStyleSheet("color:#000000; padding-left: "+QString::number(BULLET_SPACE+TEXTBOX_X_OFFSET)+"px; background-color:rgba(255,255,255,255);border: 1px solid rgba(200,200,200,255);font-size:"+QString::number(FONT_SIZE_TITLE)+"px;");
		break;
	case MariaTask::TIMED:
		_taskTypeIconHandler = new QPixmap("./Resources/ui_task_type_timed.png");
		_displayTitle->setStyleSheet("color:#000000; padding-left: "+QString::number(BULLET_SPACE+TEXTBOX_X_OFFSET)+"px; background-color:rgba(255,255,255,255);border: 1px solid rgba(200,200,200,255);font-size:"+QString::number(FONT_SIZE_TITLE)+"px;");
		break;
	default:
		_taskTypeIconHandler = new QPixmap("./Resources/ui_task_type_default.png");
		_displayTitle->setStyleSheet("color:#000000; padding-left: "+QString::number(BULLET_SPACE+TEXTBOX_X_OFFSET)+"px; background-color:rgba(255,255,255,255);border: 1px solid rgba(200,200,200,255);font-size:"+QString::number(FONT_SIZE_TITLE)+"px;");
		break;
	}
	_typeOfTask->setPixmap(*_taskTypeIconHandler);
	_typeOfTask->hide();
	_displayTitle->setText(QString::fromStdString(_taskReference->getTitle()));
	_displayTitle->setAlignment(Qt::AlignLeft);
	_displayTitle->hide();
	_timeText->setAlignment(Qt::AlignRight);
	_timeText->setStyleSheet("color:#000000;font-size:"+QString::number(FONT_SIZE_TITLE)+"px; padding-right: "+QString::number(TEXTBOX_X_OFFSET)+"px;");
	_timeText->hide();
	_desciptionText->setText(QString::fromStdString(_taskReference->getDescription()));
	_desciptionText->setStyleSheet("border: 1px solid black; color:#000000;font-size:"+QString::number(FONT_SIZE_DESCRIPTION)+"px; padding-right: "+QString::number(TEXTBOX_X_OFFSET)+"px;");
	_desciptionText->setAlignment(Qt::AlignLeft);
	_desciptionText->setWordWrap(true);
	_desciptionText->hide();

	_width=width;

	_updatePositionTimer = new QTimer(this);
	connect(_updatePositionTimer, SIGNAL(timeout()), this, SLOT(updatePosition()));
	_updatePositionTimer->start(1);

	if(_currentType==NORMAL) {
		_updateTimeTextTimer = new QTimer(this);
		connect(_updateTimeTextTimer, SIGNAL(timeout()), this, SLOT(updateTimeText()));
		updateTimeText();
	}
}

MariaUITask::~MariaUITask() {
	if(_currentType==NORMAL&&_updateTimeTextTimer->isActive()) {
		_updateTimeTextTimer->stop();
	}

	if(_updatePositionTimer->isActive()) {
		_updatePositionTimer->stop();
	}

	if(_currentType==NORMAL) {
		delete _updateTimeTextTimer;
	}

	delete _updatePositionTimer;
	delete _taskTypeIconHandler;
	delete _typeOfTask;
	delete _desciptionText;
	delete _timeText;
	delete _displayTitle;
}

bool MariaUITask::setTitlePretext(string pretext) {
	if(_taskReference!=NULL) {
		_displayTitle->setText(QString::fromStdString(pretext)+QString::fromStdString(_taskReference->getTitle()));
		return true;
	} else {
		return false;
	}
}

bool MariaUITask::updatePosition() {
	if(abs(_position.x()-_destination.x())>VALUE_THRESHOLD||
		abs(_position.y()-_destination.y())>VALUE_THRESHOLD) {
			_position.setX(_position.x()+(_destination.x()-_position.x())*FLOW_FACTOR);
			_position.setY(_position.y()+(_destination.y()-_position.y())*FLOW_FACTOR);
			switch(_currentType) {
			case NORMAL:
				_displayTitle->setGeometry(QRect(_position.x(),_position.y(),_width,TASK_HEIGHT));
				_timeText->setGeometry(QRect(_position.x()+TIMESTAMP_X_OFFSET,_position.y(),_width,TASK_HEIGHT));
				break;
			case EXPANDED:
				_displayTitle->setGeometry(QRect(_position.x(),_position.y(),_width,TASK_HEIGHT_EXPANDED));
				_desciptionText->setGeometry(QRect(_position.x()+TEXTBOX_X_OFFSET+DESCRIPTION_X_OFFSET,_position.y()+DESCRIPTION_Y_OFFSET,_width-(TEXTBOX_X_OFFSET+DESCRIPTION_X_OFFSET)*2,TASK_HEIGHT_EXPANDED-DESCRIPTION_Y_OFFSET*2));
				break;
			}
			_typeOfTask->setGeometry(QRect(_position.x()+TEXTBOX_X_OFFSET+BULLET_X_OFFSET,_position.y()+BULLET_Y_OFFSET,_taskTypeIconHandler->width(),_taskTypeIconHandler->height()));
			return true;
	} else {
		if(_updatePositionTimer->isActive()) {
			_updatePositionTimer->stop();
		}
		return false;
	}
}

void MariaUITask::updateTimeText() {
	if(_taskReference!=NULL&&_currentType==NORMAL) {
		string timeFormatted = _taskReference->getTimeFromNow();

		if(_taskReference->getType() == MariaTask::DEADLINE) {
			if(timeFormatted.empty()){
				timeFormatted = MESSAGE_DEADLINETASK_OVERDUE;
			}else{
				timeFormatted = MESSAGE_DEADLINETASK_DUE + timeFormatted;
			}
			_timeText->setText(QString::fromStdString(timeFormatted));

		}else if(_taskReference->getType() == MariaTask::TIMED){
			if(timeFormatted.empty()){
				timeFormatted = MESSAGE_TIMEDTASK_AFTER;
			}else{
				timeFormatted = MESSAGE_TIMEDTASK_BEFORE + timeFormatted;
			}
		}
		_timeText->setText(QString::fromStdString(timeFormatted));
	}	
}

void MariaUITask::setPosition(QPointF position) {
	_position=position;
	if(!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(1);
	}
}

QPointF MariaUITask::getPosition() {
	return _position;
}

void MariaUITask::setDestination(QPointF destination) {
	_destination=destination;
	if(!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(1);
	}
}

QPointF MariaUITask::getDestination() {
	return _destination;
}

bool MariaUITask::updateDetails() {
	if(_taskReference!=NULL) {
		_displayTitle->setText(QString::fromStdString(_taskReference->getTitle()));
		if(_currentType==EXPANDED) {
			_desciptionText->setText(QString::fromStdString(_taskReference->getDescription()));
		}
		return true;
	} else {
		return false;
	}
}

void MariaUITask::stopUpdatingTime() {
	if(_currentType==NORMAL&&_updateTimeTextTimer->isActive()) {
		_updateTimeTextTimer->stop();
	}
}

void MariaUITask::startUpdatingTime() {
	if(_currentType==NORMAL&&!_updateTimeTextTimer->isActive()) {
		_updateTimeTextTimer->start(TIME_UPDATE_FREQUENCY);
	}
}

void MariaUITask::show() {
	_displayTitle->show();
	_timeText->show();
	_typeOfTask->show();
	if(_currentType==EXPANDED) {
		_desciptionText->show();
	}
}

void MariaUITask::hide() {
	_displayTitle->hide();
	_timeText->hide();
	_typeOfTask->hide();
	if(_currentType==EXPANDED) {
		_desciptionText->hide();
	}
}


float MariaUITask::getTaskHeight() {
	float toReturn;

	switch(_currentType) {
	case NORMAL:
		toReturn=TASK_HEIGHT;
		break;
	case EXPANDED:
		toReturn=TASK_HEIGHT_EXPANDED;
		break;
	}
	return toReturn;
}

MariaTask * MariaUITask::getMariaTask() {
	return _taskReference;
}