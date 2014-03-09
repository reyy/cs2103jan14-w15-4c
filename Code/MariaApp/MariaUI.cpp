#include <QKeyEvent>
#include "MariaUI.h"
#include "MariaLogic.h"
#include "MariaInterpreter.h"

const float MariaUI::FLOW_FACTOR=0.1;
const float MariaUI::VALUE_THRESHOLD=0.5;
const float MariaUI::CLOSE_BUTTON_X_OFFSET=0.0;
const float MariaUI::CLOSE_BUTTON_Y_OFFSET=0.0;
const float MariaUI::WINDOW_DEFAULT_COLOR_R=186;
const float MariaUI::WINDOW_DEFAULT_COLOR_G=199;
const float MariaUI::WINDOW_DEFAULT_COLOR_B=22;

MariaUI::MariaUI(MariaLogic *mariaLogic, QWidget *parent) : QMainWindow(parent) {
	_mariaLogic = mariaLogic;

	initWindow();
	initButtons();
	initBackgroundColor(WINDOW_DEFAULT_COLOR_R,WINDOW_DEFAULT_COLOR_G,WINDOW_DEFAULT_COLOR_B);
	_commandBar = new MariaUICommandBar(this);
	_commandBar->getTextbox()->setFocus();
	show();
}

MariaUI::~MariaUI() {
	delete _btClose;	
	delete _commandBar;
}

void MariaUI::initWindow() {
	_expandView=false;

	if (objectName().isEmpty()) {
		setObjectName(QStringLiteral("MariaUI"));
	}
	setWindowFlags( Qt::FramelessWindowHint );
	setWindowTitle(QApplication::translate("MariaUI", "M.A.R.I.A", 0));

	resize(WINDOW_DEFAULT_SIZE_X, WINDOW_DEFAULT_SIZE_Y);
}

void MariaUI::initButtons() {
	QPixmap imageHandleCloseButton("./Resources/ui_bt_close.png");
	_btClose = new QToolButton(this);
	_btClose->setAutoFillBackground(true);;
	_btClose->setIcon(QIcon(imageHandleCloseButton));
	_btClose->setIconSize(QSize(imageHandleCloseButton.width(),imageHandleCloseButton.height()));
	_btClose->setFixedSize(QSize(imageHandleCloseButton.width(),imageHandleCloseButton.height()));
	_btClose->setToolTip("Close Program");
	_btClose->setStyleSheet("border:0px;");
	_btClose->setGeometry(QRect(width()-imageHandleCloseButton.width()+CLOSE_BUTTON_X_OFFSET, CLOSE_BUTTON_Y_OFFSET, imageHandleCloseButton.width(), imageHandleCloseButton.height()));	

	connect(_btClose, SIGNAL(clicked()),this , SLOT(quitAction()));
}

void MariaUI::initBackgroundColor(float r, float g, float b) {
	_colorR=_colorTargetR=r;
	_colorG=_colorTargetG=g;
	_colorB=_colorTargetB=b;

	_bkgColorUpdateTimer = new QTimer(this);
    connect(_bkgColorUpdateTimer, SIGNAL(timeout()), this, SLOT(updateBackgroundColor()));
	updateBackgroundColor();
}

/*
void MariaUI::updateStatePreAnimation() {
	bool canstop=false;

	switch(_currentState) {
	case CALENDAR:
	case HOME:
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
	case CALENDAR:
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
	case HOME:
		if(abs(_rollingCoord.y()-_destCoord.y())>0.5) {
			_rollingCoord.setY(_rollingCoord.y()+(_destCoord.y()-_rollingCoord.y())*0.01);
			updateGUI();
		} else {
			if(_home->isEndRollingDone()) {
				_home->clearActiveDisplay();
				_rollingCoord.setY(_destCoord.y());
				canstop=true;
			}
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
*/
void MariaUI::quitAction() {
	_mariaLogic->processCommand("quit");
}

void MariaUI::updateBackgroundColor() {
	if(abs(_colorR-_colorTargetR)>VALUE_THRESHOLD ||
	   abs(_colorG-_colorTargetG)>VALUE_THRESHOLD ||
	   abs(_colorB-_colorTargetB)>VALUE_THRESHOLD) {
		_colorR+=(_colorTargetR-_colorR)*FLOW_FACTOR;
		_colorG+=(_colorTargetG-_colorG)*FLOW_FACTOR;
		_colorB+=(_colorTargetB-_colorB)*FLOW_FACTOR;
	} else {
		if(_bkgColorUpdateTimer->isActive()) {
			_bkgColorUpdateTimer->stop();
		}
	}
	QString backgroundcolor=QString::number(_colorR)+","+QString::number(_colorG)+","+QString::number(_colorB);
		this->setStyleSheet("QMainWindow  {background-color: rgb("+backgroundcolor+");min-width:400px;min-height:120px;}");
}

void MariaUI::resizeEvent(QResizeEvent* event) {
	QWidget::resizeEvent(event);
}

void MariaUI::keyReleaseEvent(QKeyEvent* keyevent){
	int keyPressed = keyevent->key();

	if(keyPressed == Qt::Key_Return || keyPressed == Qt::Key_Enter){
		_mariaLogic->processCommand(_commandBar->getTextbox()->getUserInput());
	}else{
		//todo: tick / question if keyword detected
		_commandBar->getStatus()->setStatus(MariaUIStatus::WAIT);
		_commandBar->getTextbox()->setSuggestText("");
	}
}
/*
void MariaUI::beginNewState() {
	_currentState=_stateQueue.front();
	_stateQueue.pop();

	switch(_currentState) {
	case CALENDAR: {
			_calendar->createUI(MariaUICalendar::DEFAULT);
				//TO DO - update to allow users to choose a view type.
		
			vector<MariaTask*> tempList = _mariaTaskManager->findTask("");
				for(MariaTask* temp : tempList){
					_calendar->addDisplay(*temp);
				}
			_calendar->startRolling();
			}
	case SETTING:
		_destCoord.setY(30);
		break;
	case HOME:{
			_home->createUI();
			_home->startRolling();
			_destCoord.setY(height()*0.5-10);
		}
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
		case CALENDAR:
			_calendar->endRolling();
			//Checks if the state you are going to is within the environment itself.
			if(_stateQueue.front()!=CALENDAR&&
				_stateQueue.front()!=SETTING&&
				_stateQueue.front()!=HOME) {
				_destCoord.setY(-WINDOW_DEFAULT_SIZE_Y);
			}
			break;
		case HOME:
			_home->endRolling();
			//Checks if the state you are going to is within the environment itself.
			if(_stateQueue.front()!=CALENDAR&&
				_stateQueue.front()!=SETTING&&
				_stateQueue.front()!=HOME) {
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
*/

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

void MariaUI::setBackgroundColor(float r, float g, float b) {
	_colorTargetR=r;
	_colorTargetG=g;
	_colorTargetB=b;

	if(!_bkgColorUpdateTimer->isActive()) {
		_bkgColorUpdateTimer->start(50);
	}
}

MariaUICommandBar * MariaUI::getCommandBar() {
	return _commandBar;
}