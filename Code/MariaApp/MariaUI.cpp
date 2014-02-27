#include "MariaUI.h"
#include "MariaLogic.h"
#include <QkeyEvent>

MariaUI::MariaUI(MariaLogic *mariaLogic, QWidget *parent) : QMainWindow(parent)
{
	this->mariaLogic = mariaLogic;

	initState();
	initImages();
	initWindowTitle();
	initTextBox();
	initStatusIcon();
	initButtons();
	initLoading();

	show();
}
MariaUI::~MariaUI(void) {
	delete _mariaUILoading;
	delete _statusAnimationTimer;
	delete _statePreAnimationTimer;
	delete _statePosAnimationTimer;
	delete _statusIcon;
	delete _suggestText;
	delete _questionText;
	delete _btClose;
	//TODO: Hacked this to fix runtime error. Jay please change :)
	for(int i=0;i</*AMOUNT_OF_IMAGES*/7;i++) {
		delete _imageHandle[i];
	}
	delete _imageHandle;
	delete _inputBox;
}

void MariaUI::initState() {
	_stateTargetY=-WINDOW_DEFAULT_SIZE_Y*0.5;
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

void MariaUI::initWindowTitle() {
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

void MariaUI::initLoading() {
	_mariaUILoading = new MariaUILoading(this);
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
	case DEFAULT:
	case FOCUS:
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
	}
}
void MariaUI::updateStatePosAnimation() {
	bool canstop=false;
	switch(_currentState)
	{
	case DEFAULT:
	case FOCUS:
		if(abs(_toolBoxCoordinate.y()-_stateTargetY)>0.5) {
			_toolBoxCoordinate.setY(_toolBoxCoordinate.y()+(_stateTargetY-_toolBoxCoordinate.y())*0.01);
			updateGUI();
		} else {
			canstop=true;
			_toolBoxCoordinate.setY(_stateTargetY);
		}
		break;
	case INTRO:
		if(_mariaUILoading->isAnimationDone()) {
			canstop=true;
		}
		break;
	default:
		canstop=true;
		break;
	}
	
	if(canstop) {
		_statePosAnimationTimer->stop();
		setInternalState();
	}
}

void MariaUI::resizeEvent(QResizeEvent* event) {
	updateGUI();
	QWidget::resizeEvent(event);
}

/*
Function starts the new state and its animation before hand.
*/
void MariaUI::setInternalState() {
	_currentState=_queueState;
	switch(_currentState) {
	case DEFAULT:
		_stateTargetY=height()*0.5-10;
		break;
	case FOCUS:
		_stateTargetY=25;
		break;
	case INTRO:
		setStatus(NONE);
		_mariaUILoading->startLoadingAnimation();
		break;
	default:
		break;
	}

	updateStatePreAnimation();

	if(!_statePreAnimationTimer->isActive())
		_statePreAnimationTimer->start(1);
}

void MariaUI::keyReleaseEvent(QKeyEvent* keyevent){
	int keyPressed = keyevent->key();

	if(keyPressed == Qt::Key_Return || keyPressed == Qt::Key_Enter){
		mariaLogic->processCommand(_inputBox->text());
	}else{
		//todo: tick / question if keyword detected
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
	_currentStatus=type;
	updateStatusAnimation();
	if(!_statusAnimationTimer->isActive())
		_statusAnimationTimer->start(500);
}

MariaUI::STATUS_TYPE MariaUI::getStatus() {
	return _currentStatus;
}

/*
Function cleans up the animation for previous states
before starting the new state.
*/
bool MariaUI::setState(STATE_TYPE state) {
	if(_queueState==_currentState&&_currentState!=state) {
		_queueState=state;

		switch(_currentState) {
		case DEFAULT:
		case FOCUS:
			_stateTargetY=-WINDOW_DEFAULT_SIZE_Y;
			break;
		case INTRO:
			_mariaUILoading->endLoadingAnimation();
			break;
		default:
			break;
		}

		updateStatePosAnimation();
		if(!_statePosAnimationTimer->isActive())
			_statePosAnimationTimer->start(1);

		return true;
	} else {
		return false;
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

void MariaUI::setBackgroundColor(const QString text) {
	_backgroundColor=text;
	this->setStyleSheet("QMainWindow  {background-color: "+text+";min-width:400px;min-height:120px;}");
}