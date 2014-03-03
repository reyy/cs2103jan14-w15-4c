#include "MariaUICalendar.h"

displayPack::displayPack(QMainWindow *parent) {
	_parent=parent;

	displayTitle = new QLabel(_parent);
	displayTitle->setStyleSheet("color:#ffffff;font-size:8px");
	displayTitle->hide();

	backgroundColor="#ff0000";

	_destinationX=0;
	_destinationY=0;
	_x=0;
	_y=0;
}

displayPack::displayPack(QMainWindow *parent, QString title,MariaTask::TaskType type, time_t start,time_t end) {
	_parent=parent;

	displayTitle = new QLabel(_parent);
	displayTitle->setStyleSheet("color:#ffffff; border: 1px solid grey;");
	displayTitle->setText(title);
	displayTitle->hide();

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
	displayTitle->setGeometry(QRect(layerX+_x,layerY+_y,100,12));
}

void displayPack::show() {
	displayTitle->show();
}

void displayPack::hide() {
	displayTitle->hide();
}

MariaUICalendar::MariaUICalendar(QMainWindow *parent) : MariaUIRolling(parent) {
	_parent=parent;
}

MariaUICalendar::~MariaUICalendar() {
	clearActiveDisplay();
	clearQueueDisplay();
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

void MariaUICalendar::addDisplay(MariaTask task) {

	displayPack* newDisplay=new displayPack(_parent,QString(task.getTitle().c_str()),task.getType(),task.getStart(),task.getEnd());
	newDisplay->setDestinationX(-_parent->width()*0.5+30);
	newDisplay->setDestinationY(-20+(int)_queuedisplayQueue.size()*14);
	newDisplay->setRealX(_parent->width()*0.5+10+(int)_queuedisplayQueue.size()*10);
	newDisplay->setRealY(-20+(int)_queuedisplayQueue.size()*14);
	newDisplay->hide();

	_queuedisplayQueue.push(newDisplay);
	startMainAnimationTimer();
}

void MariaUICalendar::clearActiveDisplay() {
	while(_displayPackStack.size()>0) {
		delete _displayPackStack.back();
		_displayPackStack.pop_back();
	}
}

void MariaUICalendar::updateGUI() {
	for(int i=0;i<_displayPackStack.size();i++) {
		_displayPackStack.at(i)->updateGUI(getRollingX(),getRollingY());
	}
}