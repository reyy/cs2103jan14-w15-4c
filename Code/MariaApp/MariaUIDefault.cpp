#include "MariaUIDefault.h"

MariaUIDefault::MariaUIDefault(QMainWindow *parent) : MariaUIRolling(parent) {
	_parent=parent;
	_calendarUnit=_parent->width();
	initImages();
}

MariaUIDefault::~MariaUIDefault() {
	clearActiveDisplay();
	clearQueueDisplay();
	delete _currentTimeLineImage;
}

void MariaUIDefault::initImages() {
}

void MariaUIDefault::updateStateMainAnimation() {
	bool canEnd=true;

	//Pop all queued task from addTask into the actual stack
	//for display.
	if(getCurrentState()==DURING) {
		while(_queuedisplayQueue.size()>0) {
			MariaUIDisplayPack* temp=_queuedisplayQueue.front();
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

void MariaUIDefault::clearQueueDisplay() {
	while(_queuedisplayQueue.size()>0) {
		delete _queuedisplayQueue.front();
		_queuedisplayQueue.pop();
	}
}

void MariaUIDefault::createUI(VIEW_TYPE type) {
}

void MariaUIDefault::addDisplay(MariaTask task) {

	MariaUIDisplayPack* displayPack=new MariaUIDisplayPack(_parent,task,_calendarUnit);
	displayPack->setDestinationX(-_parent->width()*0.5+30);
	displayPack->setDestinationY(-20+(int)_queuedisplayQueue.size()*14);
	displayPack->setRealX(_parent->width()*0.5+10+(int)_queuedisplayQueue.size()*10);
	displayPack->setRealY(-20+(int)_queuedisplayQueue.size()*14);
	displayPack->hide();

	_queuedisplayQueue.push(displayPack);
	startMainAnimationTimer();
}

void MariaUIDefault::clearActiveDisplay() {
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

void MariaUIDefault::updateGUI() {
	_currentTimeLine->setGeometry(QRect(getRollingX()-6,getRollingY()-25,12,78));
	for(int i=0;i<_lineStack.size();i++) {
		_lineStack.at(i)->setGeometry(QRect(30+_calendarUnit*i+getRollingX()-_parent->width()*0.5-1,getRollingY()-12,2,60));
	}

	for(int i=0;i<_lineTimerStack.size();i++) {
		_lineTimerStack.at(i)->setGeometry(QRect(30+_calendarUnit*i+getRollingX()-_parent->width()*0.5-15,getRollingY()+53,30,16));
	}

	for(int i=0;i<_displayPackStack.size();i++) {
		_displayPackStack.at(i)->updateGUI(getRollingX(),getRollingY());
	}
}