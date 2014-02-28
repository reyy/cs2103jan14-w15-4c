#include <QKeyEvent>
#include "MariaUI.h"
#include "MariaLogic.h"
#include "MariaInterpreter.h"

MariaUI::MariaUI(MariaLogic *mariaLogic, QWidget *parent) : QMainWindow(parent) {
	this->_mariaLogic = mariaLogic;

	initState();
	initImages();
	initWindow();
	initTextBox();
	initStatusIcon();
	initButtons();
	initLayers();

	show();
}

MariaUI::~MariaUI(void) {
	delete _loading;
	delete _calendar;
	delete _statusAnimationTimer;
	delete _statePreAnimationTimer;
	delete _statePosAnimationTimer;
	delete _statusIcon;
	delete _suggestText;
	delete _questionText;
	delete _btClose;

	for(int i=0;i<AMOUNT_OF_IMAGES;i++) {
		delete _imageHandle[i];
	}
	delete _imageHandle;
	delete _inputBox;
}

void MariaUI::initState() {
	_stateTargetY=-WINDOW_DEFAULT_SIZE_Y*0.5;
	_processingState=false;
	_toolBoxCoordinate.setX(30.0);
	_toolBoxCoordinate.setY(-WINDOW_DEFAULT_SIZE_Y*0.5);

	_statePreAnimationTimer = new QTimer(this);
    connect(_statePreAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStatePreAnimation()));

	_statePosAnimationTimer = new QTimer(this);
    connect(_statePosAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStatePosAnimation()));
}

void MariaUI::initImages() {
	
	_imageHandle = new QPixmap*[AMOUNT_OF_IMAGES];
	_imageHandle[0] = new QPixmap("./Resources/ui_status_recognise.png");
	_imageHandle[1] = new QPixmap("./Resources/ui_status_unknown.png");
	_imageHandle[2] = new QPixmap("./Resources/ui_status_wait_0.png");
	_imageHandle[3] = new QPixmap("./Resources/ui_status_wait_1.png");
	_imageHandle[4] = new QPixmap("./Resources/ui_status_wait_2.png");
	_imageHandle[5] = new QPixmap("./Resources/ui_status_wait_3.png");
	_imageHandle[6] = new QPixmap("./Resources/ui_bt_close.png");
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

void MariaUI::initTextBox() {
	_inputBox = new QLineEdit(this);
	_inputBox->setStyleSheet("background-color: #ffffff;border:0px;border-radius: 5px;");
	_inputBox->installEventFilter(this);

	_suggestText = new QLabel(this);
	_suggestText->setStyleSheet("background-color: rgba(0,0,0,0);border:0px;");	

	_questionText = new QLabel(this);
	_questionText->setStyleSheet("background-color: rgba(0,0,0,0);border:0px;color:#ffffff;font-weight:bold;");	
}

void MariaUI::initStatusIcon(){
	_statusImageIndex=0;
	_statusIcon = new QLabel(this);
	_statusIcon->setPixmap(*_imageHandle[_statusImageIndex]);
	_statusIcon->setStyleSheet("");
	_statusIcon->hide();

	_statusAnimationTimer = new QTimer(this);
    connect(_statusAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStatusAnimation()));
}

void MariaUI::initButtons() {
	_btClose = new QToolButton(this);
	_btClose->setAutoFillBackground(true);;
	_btClose->setIcon(QIcon(*_imageHandle[6]));
	_btClose->setIconSize(QSize(20,20));
	_btClose->setFixedSize(QSize(20,20));
	_btClose->setStyleSheet("border:0px;");	
	_btClose->setToolTip("Close Program");

	//To fix the quick button.
	connect(_btClose, SIGNAL(clicked()),this , SLOT(quit()));
}

void MariaUI::initLayers() {
	_loading = new MariaUILoading(this);
	_calendar = new MariaUICalendar(this);
}

void MariaUI::updateStatusAnimation() {
	_statusIcon->show();
	switch(_currentStatus) {
	case OK:
		_statusAnimationTimer->stop();
		_statusImageIndex=0;
		break;
	case INVALID:
		_statusAnimationTimer->stop();
		break;
	case WAIT:
		if(_statusImageIndex<2||_statusImageIndex>5)
			_statusImageIndex=2;
		if(_statusImageIndex<5)
			_statusImageIndex++;
		else
			_statusImageIndex=2;
		break;
	case UNKNOWN:
		_statusAnimationTimer->stop();
		_statusImageIndex=1;
		break;
	case NONE:
		_statusAnimationTimer->stop();
		_statusIcon->hide();
		break;
	default:
		break;
	}
	
	_statusIcon->setPixmap(*_imageHandle[_statusImageIndex]);
}

void MariaUI::updateStatePreAnimation() {
	bool canstop=false;
	switch(_currentState)
	{
	case FOCUS_CALENDAR:
	case DEFAULT:
		if(abs(_toolBoxCoordinate.y()-_stateTargetY)>0.5) {
			_toolBoxCoordinate.setY(_toolBoxCoordinate.y()+(_stateTargetY-_toolBoxCoordinate.y())*0.01);
			updateGUI();
		} else {
			canstop=true;
			_toolBoxCoordinate.setY(_stateTargetY);
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
	switch(_currentState)
	{
	case FOCUS_CALENDAR:
		if(abs(_toolBoxCoordinate.y()-_stateTargetY)>0.5) {
			_toolBoxCoordinate.setY(_toolBoxCoordinate.y()+(_stateTargetY-_toolBoxCoordinate.y())*0.01);
			updateGUI();
		} else {
			if(_calendar->isEndRollingDone()) {
				_calendar->clearActiveDisplay();
				_toolBoxCoordinate.setY(_stateTargetY);
				canstop=true;
			}
		}
		break;
	case DEFAULT:
		if(abs(_toolBoxCoordinate.y()-_stateTargetY)>0.5) {
			_toolBoxCoordinate.setY(_toolBoxCoordinate.y()+(_stateTargetY-_toolBoxCoordinate.y())*0.01);
			updateGUI();
		} else {
			canstop=true;
			_toolBoxCoordinate.setY(_stateTargetY);
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

void MariaUI::resizeEvent(QResizeEvent* event) {
	updateGUI();
	QWidget::resizeEvent(event);
}

void MariaUI::keyReleaseEvent(QKeyEvent* keyevent){
	int keyPressed = keyevent->key();

	if(keyPressed == Qt::Key_Return || keyPressed == Qt::Key_Enter){
		_mariaLogic->processCommand(_inputBox->text());
	}else{
		//todo: tick / question if keyword detected
		_suggestText->setText("");
		setStatus(WAIT);
	}
}

void MariaUI::beginNewState() {
	_currentState=_stateQueue.front();
	_stateQueue.pop();

	switch(_currentState) {
	case FOCUS_CALENDAR:
		_calendar->startRolling();
	case FOCUS_SETTING:
		_stateTargetY=25;
		break;
	case DEFAULT:
		_stateTargetY=height()*0.5-10;
		break;
	case INTRO:
		setStatus(NONE);
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
				_stateTargetY=-WINDOW_DEFAULT_SIZE_Y;
			}
			break;
		case INTRO:
			_stateTargetY=-WINDOW_DEFAULT_SIZE_Y;
			_loading->endLoadingAnimation();
			break;
		default:
			_stateTargetY=-WINDOW_DEFAULT_SIZE_Y;
			break;
		}
		if(!_statePosAnimationTimer->isActive())
			_statePosAnimationTimer->start(1);
	}
}

void MariaUI::updateGUI() {
	_statusIcon->setGeometry(QRect(_toolBoxCoordinate.x(), _toolBoxCoordinate.y(), 20, 20));
	_inputBox->setGeometry(QRect(_toolBoxCoordinate.x()+30, _toolBoxCoordinate.y(), width()-_toolBoxCoordinate.x()-70, 20));
	_suggestText->setGeometry(QRect(_toolBoxCoordinate.x()+33, _toolBoxCoordinate.y(), width()-_toolBoxCoordinate.x()-70, 20));
	_questionText->setGeometry(QRect(_toolBoxCoordinate.x()+33, _toolBoxCoordinate.y()-20, width()-_toolBoxCoordinate.x()-75, 20));
	_btClose->setGeometry(QRect(width()-10-10, 10-10, 20, 20));	
}

void MariaUI::setStatus(STATUS_TYPE type) {
	if(_currentStatus!=type) {
		_currentStatus=type;
		updateStatusAnimation();
		if(!_statusAnimationTimer->isActive())
			_statusAnimationTimer->start(500);
	}
}

MariaUI::STATUS_TYPE MariaUI::getStatus() {
	return _currentStatus;
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

void MariaUI::setBaseText(const QString text) {
	_suggestText->setText(text);
}

void MariaUI::setQuestionText(const QString text) {
	_questionText->setText(text);
}

QString MariaUI::getUserInput() {
	return _inputBox->text();
}

void MariaUI::setUserInput(const QString text) {
	_inputBox->setText(text);
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