#include "MariaUITask.h"
#include "MariaTime.h"

MariaUITask::MariaUITask(QMainWindow *parent, MariaTask task, float calendarUnit) {
	_parent=parent;

	_displayTitle = new QLabel(_parent);
	switch(task.getType()) {
	case MariaTask::FLOATING:
		_displayTitle->setStyleSheet("color:#ffffff; background-color:rgba(255,0,0,128);border: 1px solid white;");
		break;
	case MariaTask::DEADLINE:
		_displayTitle->setStyleSheet("color:#ffffff; background-color:rgba(255,255,0,128);border: 1px solid white;");
		break;
	case MariaTask::TIMED:
		_displayTitle->setStyleSheet("color:#ffffff; background-color:rgba(255,255,128,128);border: 1px solid white;");
		break;
	default:
		_displayTitle->setStyleSheet("color:#ffffff; background-color:rgba(0,0,255,128);border: 1px solid white;");
		break;
	}
	_displayTitle->setText(QString::fromStdString(task.getTitle()));
	_displayTitle->setAlignment(Qt::AlignCenter);
	_displayTitle->hide();


	_calendarUnit=5*3600/60/60*calendarUnit;

	_destinationX=0;
	_destinationY=0;
	_x=0;
	_y=0;
}

MariaUITask::~MariaUITask() {
	delete _displayTitle;
}

void MariaUITask::setDestinationX(float x) {
	_destinationX=x;
}

void MariaUITask::setDestinationY(float y) {
	_destinationY=y;
}

void MariaUITask::setRealX(float x) {
	_x=x;
}

void MariaUITask::setRealY(float y) {
	_y=y;
}

bool MariaUITask::isCoordinateMatch() {
	if(abs(_x-_destinationX)<0.5&&abs(_y-_destinationY)<0.5) {
		return true;
	} else {
		return false;
	}
}

bool MariaUITask::updatePosition() {
	if(!isCoordinateMatch()) {
		_x+=(_destinationX-_x)*0.01;
		_y+=(_destinationY-_y)*0.01;
		return true;
	} else {
		return false;
	}
}

void MariaUITask::updateGUI(float layerX,float layerY) {
	_displayTitle->setGeometry(QRect(layerX+_x,layerY+_y,_calendarUnit,14));
}

void MariaUITask::show() {
	_displayTitle->show();
}

void MariaUITask::hide() {
	_displayTitle->hide();
}