#include "MariaUIHome.h"

MariaUIHome::MariaUIHome(QMainWindow *parent) : MariaUIRolling(parent) {
	_parent=parent;
	initImages();
}

MariaUIHome::~MariaUIHome() {
	clearActiveDisplay();
	clearQueueDisplay();
	delete _currentTime;
}

void MariaUIHome::initImages() {
}

void MariaUIHome::updateStateMainAnimation() {
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

void MariaUIHome::clearQueueDisplay() {
	while(_queuedisplayQueue.size()>0) {
		delete _queuedisplayQueue.front();
		_queuedisplayQueue.pop();
	}
}

void MariaUIHome::createUI() {
}

void MariaUIHome::addDisplay(MariaTask task) {

	MariaUIDisplayPack* displayPack=new MariaUIDisplayPack(_parent,task,100);
	displayPack->setDestinationX(-_parent->width()*0.5+30);
	displayPack->setDestinationY(-20+(int)_queuedisplayQueue.size()*14);
	displayPack->setRealX(_parent->width()*0.5+10+(int)_queuedisplayQueue.size()*10);
	displayPack->setRealY(-20+(int)_queuedisplayQueue.size()*14);
	displayPack->hide();

	_queuedisplayQueue.push(displayPack);
	startMainAnimationTimer();
}

void MariaUIHome::clearActiveDisplay() {
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

void MariaUIHome::updateGUI() {
	for(int i=0;i<_displayPackStack.size();i++) {
		_displayPackStack.at(i)->updateGUI(getRollingX(),getRollingY());
	}
}