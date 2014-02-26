#include "MariaUI.h"

MariaUI::MariaUI(QWidget *parent) : QMainWindow(parent)
{
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
	delete _statusAnimationTimer;
	delete _stateAnimationTimer;
	delete _statusIcon;
	delete _suggestText;
	delete _btClose;

	for(int i=0;i<AMOUNT_OF_IMAGES;i++) {
		delete _imageHandle[i];
	}
	delete _imageHandle;
	delete _inputBox;
}

void MariaUI::initState() {
	_toolBoxCoordinate.setX(30.0);
	_toolBoxCoordinate.setY(-WINDOW_DEFAULT_SIZE_Y*0.5);

	_stateAnimationTimer = new QTimer(this);
    connect(_stateAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStateAnimation()));
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
	_inputBox->setStyleSheet("background-color: #ffffff;border:0px;");

	_suggestText = new QLabel(this);
	_suggestText->setStyleSheet("background-color: rgba(0,0,0,0);border:0px;");	
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

void MariaUI::updateStateAnimation() {
	float targetY=0;
	bool canstop=false;
	switch(_currentState)
	{
	case DEFAULT:
		targetY=height()*0.5-10;
		if(abs(_toolBoxCoordinate.y()-targetY)>0.5) {
			_toolBoxCoordinate.setY(_toolBoxCoordinate.y()+(targetY-_toolBoxCoordinate.y())*0.01);
			updateGUI();
		} else {
			canstop=true;
		}
		break;
	case FOCUS:
		targetY=20;
		if(abs(_toolBoxCoordinate.y()-targetY)>0.5) {
			_toolBoxCoordinate.setY(_toolBoxCoordinate.y()+(targetY-_toolBoxCoordinate.y())*0.01);
			updateGUI();
		} else {
			canstop=true;
		}
		break;
	case INTRO:
		targetY=-WINDOW_DEFAULT_SIZE_Y;
		if(!_mariaUILoading->isAnimationDone()) {
			_mariaUILoading->startLoadingAnimation();
		} else {
			_currentState=DEFAULT;
		}
		break;
	default:
		break;
	}
	
	if(canstop) {
		_stateAnimationTimer->stop();
		_toolBoxCoordinate.setY(targetY);
	}
}

void MariaUI::resizeEvent(QResizeEvent* event) {
	updateGUI();
	QWidget::resizeEvent(event);
}

void MariaUI::updateGUI() {
	_statusIcon->setGeometry(QRect(_toolBoxCoordinate.x(), _toolBoxCoordinate.y(), 20, 20));
	_inputBox->setGeometry(QRect(_toolBoxCoordinate.x()+30, _toolBoxCoordinate.y(), width()-_toolBoxCoordinate.x()-70, 20));
	_suggestText->setGeometry(QRect(_toolBoxCoordinate.x()+33, _toolBoxCoordinate.y(), width()-_toolBoxCoordinate.x()-70, 20));
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

void MariaUI::setState(STATE_TYPE state) {
	_currentState=state;
	updateStateAnimation();
	if(!_stateAnimationTimer->isActive())
		_stateAnimationTimer->start(1);
}

MariaUI::STATE_TYPE MariaUI::getState() {
	return _currentState;
}

void MariaUI::setBaseText(const QString text) {
	_suggestText->setText(text);
}

QString MariaUI::getUserInput() {
	return _inputBox->text();
}

void MariaUI::beginLoading() {
	setState(INTRO);
	setStatus(NONE);
}

void MariaUI::endLoading() {
	_mariaUILoading->endLoadingAnimation();
}