#include <time.h>
#include "MariaUICalendar.h"

displayPack::displayPack(QMainWindow *parent) {
	_parent=parent;

	displayTitle = new QLabel(_parent);
	displayTitle->setStyleSheet("color:#ffffff; background-color:rgba(255,0,0,128);border: 1px solid white;");
	displayTitle->setAlignment(Qt::AlignCenter);
	displayTitle->hide();
	assignedWidth=100;

	backgroundColor="#ff0000";

	_destinationX=0;
	_destinationY=0;
	_x=0;
	_y=0;
}

displayPack::displayPack(QMainWindow *parent, QString title,MariaTask::TaskType type, time_t start,time_t end) {
	_parent=parent;

	displayTitle = new QLabel(_parent);
	displayTitle->setStyleSheet("color:#ffffff; background-color:rgba(255,0,0,128);border: 1px solid white;");
	displayTitle->setText(title);
	displayTitle->setAlignment(Qt::AlignCenter);
	displayTitle->hide();
	assignedWidth=100;

	backgroundColor="#ff0000";

	_destinationX=0;
	_destinationY=0;
	_x=0;
	_y=0;
}

displayPack::~displayPack() {
	delete displayTitle;
}

void displayPack::setDestinationX(float x) {
	_destinationX=x;
}

void displayPack::setDestinationY(float y) {
	_destinationY=y;
}

void displayPack::setRealX(float x) {
	_x=x;
}

void displayPack::setRealY(float y) {
	_y=y;
}

bool displayPack::isCoordinateMatch() {
	if(abs(_x-_destinationX)<0.5&&abs(_y-_destinationY)<0.5) {
		return true;
	} else {
		return false;
	}
}

bool displayPack::updatePosition() {
	if(!isCoordinateMatch()) {
		_x+=(_destinationX-_x)*0.01;
		_y+=(_destinationY-_y)*0.01;
		return true;
	} else {
		return false;
	}
}

void displayPack::updateGUI(float layerX,float layerY) {
	displayTitle->setGeometry(QRect(layerX+_x,layerY+_y,assignedWidth,14));
}

void displayPack::show() {
	displayTitle->show();
}

void displayPack::hide() {
	displayTitle->hide();
}

MariaUICalendar::MariaUICalendar(QMainWindow *parent) : MariaUIRolling(parent) {
	_parent=parent;
	_displayUnit=_parent->width();
	initImages();
}

MariaUICalendar::~MariaUICalendar() {
	clearActiveDisplay();
	clearQueueDisplay();
	delete _currentTimeLineImage;
}

void MariaUICalendar::initImages() {
	_currentTimeLineImage=new QPixmap("./Resources/ui_current_line.png");
	_timeLineImage=new QPixmap("./Resources/ui_line.png");

	_currentTimeLine = new QLabel(_parent);
	_currentTimeLine->setPixmap(*_currentTimeLineImage);
	_currentTimeLine->setGeometry(QRect(-1000,-1000,12,98));
}

void MariaUICalendar::addLine(int amount) {
	for(int i=0;i<amount;i++) {
		QLabel* temp = new QLabel(_parent);
		temp->setPixmap(*_timeLineImage);
		temp->setGeometry(QRect(-1000,-1000,2,90));
		temp->show();
		_lineStack.push_back(temp);

		QLabel* tempText = new QLabel(_parent);
		tempText->setGeometry(QRect(-1000,-1000,2,90));
		int value=i%12;
		if(value==0) {
			value+=12;
		}
		if(i<12) {
			tempText->setText(QString::number(value)+"AM");
		} else {
			tempText->setText(QString::number(value)+"PM");
		}
		tempText->setStyleSheet("color:#ffffff;");
		tempText->setAlignment(Qt::AlignCenter);
		tempText->show();
		_lineTimerStack.push_back(tempText);

		int denominator=_lineStack.size()-1;
		if(denominator==0)
			denominator++;
		_displayUnit=(_parent->width()-60)/denominator;
	}
}

void MariaUICalendar::updateStateMainAnimation() {
	bool canEnd=true;

	//Pop all queued task from addTask into the actual stack
	//for display.
	if(getCurrentState()==DURING) {
		while(_queuedisplayQueue.size()>0) {
			displayPack* temp=_queuedisplayQueue.front();
			temp->show();
			_displayPackStack.push_back(temp);
			_queuedisplayQueue.pop();
		}
	} else {
		canEnd=false;
	}

	for(int i=0;i<_displayPackStack.size();i++) {
		bool result=_displayPackStack.at(i)->updatePosition();
		
		//Checks if there is something to update.
		//Forces the animation loop to continue running.
		if(canEnd) {
			if(result) {
				canEnd=false;
			}
		}
	}
	updateGUI();

	if(canEnd) {
		stopMainAnimationTimer();
	}
}

void MariaUICalendar::clearQueueDisplay() {
	while(_queuedisplayQueue.size()>0) {
		delete _queuedisplayQueue.front();
		_queuedisplayQueue.pop();
	}
}

void MariaUICalendar::createUI(VIEW_TYPE type) {
	switch(type) {
	case DEFAULT:
		addLine(13);
		break;
	case DAY:
		addLine(25);
		break;
	case WEEK:
		addLine(8);
		break;
	case MONTH:
		addLine(31);
		//To Do, update based on the month.
		break;
	case YEAR:
		addLine(13);
		break;
	}
}

void MariaUICalendar::addDisplay(MariaTask task) {

	displayPack* newDisplay=new displayPack(_parent,QString(task.getTitle().c_str()),task.getType(),task.getStart().getUnixTime(),task.getEnd().getUnixTime());
	newDisplay->setDestinationX(-_parent->width()*0.5+30);
	newDisplay->setDestinationY(-20+(int)_queuedisplayQueue.size()*14);
	newDisplay->setRealX(_parent->width()*0.5+10+(int)_queuedisplayQueue.size()*10);
	newDisplay->setRealY(-20+(int)_queuedisplayQueue.size()*14);
	newDisplay->hide();
	double timeDiff=1.0;
	//double timeDiff = difftime(task.getEnd(),task.getStart());
	newDisplay->assignedWidth=_displayUnit*timeDiff;

	_queuedisplayQueue.push(newDisplay);
	startMainAnimationTimer();
}

void MariaUICalendar::clearActiveDisplay() {
	while(_displayPackStack.size()>0) {
		delete _displayPackStack.back();
		_displayPackStack.pop_back();
	}

	while(_lineStack.size()>0) {
		delete _lineStack.back();
		_lineStack.pop_back();
	}

	while(_lineTimerStack.size()>0) {
		delete _lineTimerStack.back();
		_lineTimerStack.pop_back();
	}
}

void MariaUICalendar::updateGUI() {

	for(int i=0;i<_lineStack.size();i++) {
		_lineStack.at(i)->setGeometry(QRect(30+_displayUnit*i+getRollingX()-_parent->width()*0.5,getRollingY()-27,2,80));
	}

	for(int i=0;i<_lineTimerStack.size();i++) {
		_lineTimerStack.at(i)->setGeometry(QRect(30+_displayUnit*i+getRollingX()-_parent->width()*0.5-15,getRollingY()+53,30,16));
	}

	for(int i=0;i<_displayPackStack.size();i++) {
		_displayPackStack.at(i)->updateGUI(getRollingX(),getRollingY());
	}
}