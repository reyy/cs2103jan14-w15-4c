#include "MariaUITaskExpanded.h"
#include "MariaTime.h"

const float MariaUITaskExpanded::FLOW_FACTOR=0.05;
const float MariaUITaskExpanded::VALUE_THRESHOLD=1.0;
const float MariaUITaskExpanded::FONT_SIZE_TITLE=16.0;
const float MariaUITaskExpanded::FONT_SIZE_DESCRIPTION=12.0;
const float MariaUITaskExpanded::TASK_HEIGHT=150.0;
const float MariaUITaskExpanded::DESCRIPTION_X_OFFSET=30.0;
const float MariaUITaskExpanded::DESCRIPTION_Y_OFFSET=30.0;
const string MariaUITaskExpanded::MESSAGE_DEADLINETASK_DUE="Due in ";
const string MariaUITaskExpanded::MESSAGE_DEADLINETASK_OVERDUE="Overdue";
const string MariaUITaskExpanded::MESSAGE_TIMEDTASK_BEFORE="Starting in ";
const string MariaUITaskExpanded::MESSAGE_TIMEDTASK_AFTER="Event started";

MariaUITaskExpanded::MariaUITaskExpanded(QMainWindow *qmainWindow, MariaTask *task, float width) {
	_qmainWindow=qmainWindow;

	_destination=QPoint(0,0);
	_position=QPoint(0,0);

	_taskReference=task;
	_displayTitle = new QLabel(_qmainWindow);
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
	_desciptionText->setText(QString::fromStdString(_taskReference->getDescription()));
	_desciptionText->setStyleSheet("color:#000000;font-size:"+QString::number(FONT_SIZE_DESCRIPTION)+"px; padding-right: "+QString::number(TEXTBOX_X_OFFSET)+"px;");
	_desciptionText->setAlignment(Qt::AlignLeft);
	_desciptionText->hide();

	_width=width;

	_updatePositionTimer = new QTimer(this);
	connect(_updatePositionTimer, SIGNAL(timeout()), this, SLOT(updatePosition()));
	_updatePositionTimer->start(1);
}

MariaUITaskExpanded::~MariaUITaskExpanded() {
	if(_updatePositionTimer->isActive()) {
		_updatePositionTimer->stop();
	}
	delete _taskTypeIconHandler;
	delete _typeOfTask;
	delete _updatePositionTimer;
	delete _displayTitle;
	delete _desciptionText;
}

bool MariaUITaskExpanded::setTitlePretext(string pretext) {
	if(_taskReference!=NULL) {
		_displayTitle->setText(QString::fromStdString(pretext)+QString::fromStdString(_taskReference->getTitle()));
		return true;
	} else {
		return false;
	}
}

bool MariaUITaskExpanded::updatePosition() {
	if(abs(_position.x()-_destination.x())>VALUE_THRESHOLD||
		abs(_position.y()-_destination.y())>VALUE_THRESHOLD) {
			_position.setX(_position.x()+(_destination.x()-_position.x())*FLOW_FACTOR);
			_position.setY(_position.y()+(_destination.y()-_position.y())*FLOW_FACTOR);
			_displayTitle->setGeometry(QRect(_position.x(),_position.y(),_width,TASK_HEIGHT));
			_desciptionText->setGeometry(QRect(_position.x()+TEXTBOX_X_OFFSET+DESCRIPTION_X_OFFSET,_position.y()+DESCRIPTION_Y_OFFSET,_width,TASK_HEIGHT));
			_typeOfTask->setGeometry(QRect(_position.x()+TEXTBOX_X_OFFSET+BULLET_X_OFFSET,_position.y()+BULLET_Y_OFFSET,_taskTypeIconHandler->width(),_taskTypeIconHandler->height()));
			return true;
	} else {
		if(_updatePositionTimer->isActive()) {
			_updatePositionTimer->stop();
		}
		return false;
	}
}

void MariaUITaskExpanded::setPosition(QPointF position) {
	_position=position;
	if(!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(1);
	}
}

QPointF MariaUITaskExpanded::getPosition() {
	return _position;
}

void MariaUITaskExpanded::setDestination(QPointF destination) {
	_destination=destination;
	if(!_updatePositionTimer->isActive()) {
		_updatePositionTimer->start(1);
	}
}

QPointF MariaUITaskExpanded::getDestination() {
	return _destination;
}

bool MariaUITaskExpanded::updateDetails() {
	if(_taskReference!=NULL) {
		_displayTitle->setText(QString::fromStdString(_taskReference->getTitle()));
		_desciptionText->setText(QString::fromStdString(_taskReference->getDescription()));
		return true;
	} else {
		return false;
	}
}

void MariaUITaskExpanded::show() {
	_displayTitle->show();
	_desciptionText->show();
	_typeOfTask->show();
}

void MariaUITaskExpanded::hide() {
	_displayTitle->hide();
	_desciptionText->hide();
	_typeOfTask->hide();
}

MariaTask * MariaUITaskExpanded::getMariaTask() {
	return _taskReference;
}