#include "MariaUICalendar.h"

MariaUICalendar::MariaUICalendar(QMainWindow *parent) : MariaUIRolling(parent) {
	_parent=parent;
	_calendarUnit=_parent->width();
	initImages();
}

MariaUICalendar::~MariaUICalendar() {
	clearActiveDisplay();
	clearQueueDisplay();
	delete _currentTimeLineImage;
	delete _timeLineImage;
	delete _currentTimeLine;
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
		_calendarUnit=(_parent->width()-60)/denominator;
	}
}

void MariaUICalendar::updateStateMainAnimation() {
	/*bool canEnd=true;

	//Pop all queued task from addTask into the actual stack
	//for display.
	if(getCurrentState()==DURING) {
		while(_queuedisplayQueue.size()>0) {
			MariaUITask* temp=_queuedisplayQueue.front();
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
	}*/
}

void MariaUICalendar::clearQueueDisplay() {
	/*while(_queuedisplayQueue.size()>0) {
		delete _queuedisplayQueue.front();
		_queuedisplayQueue.pop();
	}*/
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
	/*
	MariaUITask* displayPack=new MariaUITask(_parent,task,_calendarUnit);
	displayPack->setDestinationX(-_parent->width()*0.5+30);
	displayPack->setDestinationY(-20+(int)_queuedisplayQueue.size()*14);
	displayPack->setRealX(_parent->width()*0.5+10+(int)_queuedisplayQueue.size()*10);
	displayPack->setRealY(-20+(int)_queuedisplayQueue.size()*14);
	displayPack->hide();

	_queuedisplayQueue.push(displayPack);
	startMainAnimationTimer();*/
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
	/*_currentTimeLine->setGeometry(QRect(getRollingX()-6,getRollingY()-25,12,78));
	for(int i=0;i<_lineStack.size();i++) {
		_lineStack.at(i)->setGeometry(QRect(30+_calendarUnit*i+getRollingX()-_parent->width()*0.5-1,getRollingY()-12,2,60));
	}

	for(int i=0;i<_lineTimerStack.size();i++) {
		_lineTimerStack.at(i)->setGeometry(QRect(30+_calendarUnit*i+getRollingX()-_parent->width()*0.5-15,getRollingY()+53,30,16));
	}

	for(int i=0;i<_displayPackStack.size();i++) {
		_displayPackStack.at(i)->updateGUI(getRollingX(),getRollingY());
	}*/
}