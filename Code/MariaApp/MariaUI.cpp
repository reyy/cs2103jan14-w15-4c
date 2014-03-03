#include <QKeyEvent>
#include "MariaUI.h"
#include "MariaLogic.h"
#include "MariaInterpreter.h"

MariaUI::MariaUI(MariaLogic *mariaLogic, MariaTaskManager *mariaTaskManager, QWidget *parent) : QMainWindow(parent) {
	_mariaLogic = mariaLogic;
	_mariaTaskManager = mariaTaskManager;
	
	initWindow();
	initState();
	initSubclasses();
	initButtons();

	show();
}

MariaUI::~MariaUI(void) {
	delete _loading;
	delete _calendar;
	delete _status;
	delete _textbox;
	delete _statePreAnimationTimer;
	delete _statePosAnimationTimer;
	delete _btClose;	
}

void MariaUI::initState() {
	_currentState=DEFAULT;
	_destCoord.setX(WINDOW_DEFAULT_SIZE_X*0.5);
	_destCoord.setY(-WINDOW_DEFAULT_SIZE_Y*0.5);
	_rollingCoord.setX(30.0);
	_rollingCoord.setY(-WINDOW_DEFAULT_SIZE_Y*0.5);
	
	_processingState=false;

	_statePreAnimationTimer = new QTimer(this);
    connect(_statePreAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStatePreAnimation()));

	_statePosAnimationTimer = new QTimer(this);
    connect(_statePosAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStatePosAnimation()));
}
void MariaUI::initWindow() {
	_expandView=false;

	if (objectName().isEmpty()) {
		setObjectName(QStringLiteral("MariaUI"));
	}

	this->setStyleSheet("QMainWindow  {background-color: #a3d635;min-width:400px;min-height:120px;}");
	setWindowFlags( Qt::FramelessWindowHint );
	setWindowTitle(QApplication::translate("MariaUI", "M.A.R.I.A", 0));

	resize(WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);
}

void MariaUI::initButtons() {
	QPixmap imageHandleCloseButton("./Resources/ui_bt_close.png");
	_btClose = new QToolButton(this);
	_btClose->setAutoFillBackground(true);;
	_btClose->setIcon(QIcon(imageHandleCloseButton));
	_btClose->setIconSize(QSize(20,20));
	_btClose->setFixedSize(QSize(20,20));
	_btClose->setStyleSheet("border:0px;");	
	_btClose->setToolTip("Close Program");
	_btClose->setGeometry(QRect(width()-10-10, 10-10, 20, 20));	

	connect(_btClose, SIGNAL(clicked()),this , SLOT(quitAction()));
}

void MariaUI::initSubclasses() {
	_loading = new MariaUILoading(this);
	_calendar = new MariaUICalendar(this);	
	_status = new MariaUIStatus(this);
	_textbox = new MariaUITextbox(this);
}

void MariaUI::updateStatePreAnimation() {
	bool canstop=false;

	switch(_currentState) {
	case FOCUS_CALENDAR:
	case DEFAULT:
		if(abs(_rollingCoord.y()-_destCoord.y())>0.5) {
			_rollingCoord.setY(_rollingCoord.y()+(_destCoord.y()-_rollingCoord.y())*0.01);
			updateGUI();
		} else {
			canstop=true;
			_rollingCoord.setY(_destCoord.y());
		}
		break;
	case INTRO:
		canstop=true;
		break;
	default:
		canstop=true;
		break;
	}
	
	if(canstop) {
		_statePreAnimationTimer->stop();
		_processingState=false;
		if(_stateQueue.size()>0) {
			endOldState();
		}
	}
}

void MariaUI::updateStatePosAnimation() {
	bool canstop=false;
	switch(_currentState) {
	case FOCUS_CALENDAR:
		if(abs(_rollingCoord.y()-_destCoord.y())>0.5) {
			_rollingCoord.setY(_rollingCoord.y()+(_destCoord.y()-_rollingCoord.y())*0.01);
			updateGUI();
		} else {
			if(_calendar->isEndRollingDone()) {
				_calendar->clearActiveDisplay();
				_rollingCoord.setY(_destCoord.y());
				canstop=true;
			}
		}
		break;
	case DEFAULT:
		if(abs(_rollingCoord.y()-_destCoord.y())>0.5) {
			_rollingCoord.setY(_rollingCoord.y()+(_destCoord.y()-_rollingCoord.y())*0.01);
			updateGUI();
		} else {
			canstop=true;
			_rollingCoord.setY(_destCoord.y());
		}
		break;
	case INTRO:
		if(_loading->isAnimationDone()) {
			canstop=true;
		}
		break;
	default:
		canstop=true;
		break;
	}
	
	if(canstop) {
		_statePosAnimationTimer->stop();
		beginNewState();
	}
}

void MariaUI::quitAction() {
	_mariaLogic->processCommand("quit");
}

void MariaUI::resizeEvent(QResizeEvent* event) {
	updateGUI();
	QWidget::resizeEvent(event);
}

void MariaUI::keyReleaseEvent(QKeyEvent* keyevent){
	int keyPressed = keyevent->key();

	if(keyPressed == Qt::Key_Return || keyPressed == Qt::Key_Enter){
		_mariaLogic->processCommand(_textbox->getUserInput());
	}else{
		//todo: tick / question if keyword detected
		_status->setStatus(MariaUIStatus::WAIT);
		_textbox->setSuggestText("");
	}
}

void MariaUI::beginNewState() {
	_currentState=_stateQueue.front();
	_stateQueue.pop();

	switch(_currentState) {
	case FOCUS_CALENDAR: {
			_calendar->createUI(MariaUICalendar::DEFAULT);
				//TO DO - update to allow users to choose a view type.
		
			vector<MariaTask*> tempList = _mariaTaskManager->findTask("");
				for(MariaTask* temp : tempList){
					_calendar->addDisplay(*temp);
				}
			_calendar->startRolling();
			}
	case FOCUS_SETTING:
		_destCoord.setY(25);
		break;
	case DEFAULT:
		_destCoord.setY(height()*0.5-10);
		break;
	case INTRO:
		_status->setStatus(MariaUIStatus::NONE);
		_loading->startLoadingAnimation();
		break;
	case QUIT:
		_mariaLogic->processCommand("quit");
	default:
		break;
	}
	if(!_statePreAnimationTimer->isActive())
		_statePreAnimationTimer->start(1);
}

void MariaUI::endOldState() {
	if(!_processingState) {
		_processingState=true;

		switch(_currentState) {
		case FOCUS_CALENDAR:
			_calendar->endRolling();
		case DEFAULT:
			if(_stateQueue.front()!=FOCUS_CALENDAR&&
				_stateQueue.front()!=FOCUS_SETTING&&
				_stateQueue.front()!=DEFAULT) {
				_destCoord.setY(-WINDOW_DEFAULT_SIZE_Y);
			}
			break;
		case INTRO:
			_destCoord.setY(-WINDOW_DEFAULT_SIZE_Y);
			_loading->endLoadingAnimation();
			break;
		default:
			_destCoord.setY(-WINDOW_DEFAULT_SIZE_Y);
			break;
		}
		if(!_statePosAnimationTimer->isActive())
			_statePosAnimationTimer->start(1);
	}
}

void MariaUI::updateGUI() {
	_status->updateGUI(_rollingCoord.x(),_rollingCoord.y());
	_textbox->updateGUI(_rollingCoord.x(),_rollingCoord.y());
}

void MariaUI::setState(STATE_TYPE state) {
	if(_stateQueue.size()>0) {
		//Ignore duplicate state change.
		if(_stateQueue.back()!=state) {
			_stateQueue.push(state);
			endOldState();
		}
	} else {
		_stateQueue.push(state);
		endOldState();
	}
}

MariaUI::STATE_TYPE MariaUI::getState() {
	return _currentState;
}

void MariaUI::setExpand(bool value) {
	_expandView=value;

	if(_expandView) {
		resize(WINDOW_DEFAULT_EXPAND_SIZE_X, WINDOW_DEFAULT_EXPAND_SIZE_Y);
	} else  {
		resize(WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);
	}
}

bool MariaUI::getExpand() {
	return _expandView;
}

void MariaUI::setBackgroundColor(const QString text) {
	_backgroundColor=text;
	this->setStyleSheet("QMainWindow  {background-color: "+text+";min-width:400px;min-height:120px;}");
}

MariaUILoading * MariaUI::getLoading() {
	return _loading;
}

MariaUICalendar * MariaUI::getCalendar() {
	return _calendar;
}

MariaUIStatus * MariaUI::getStatus() {
	return _status;
}

MariaUITextbox * MariaUI::getTextbox() {
	return _textbox;
}