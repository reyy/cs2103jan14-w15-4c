#include "MariaUIStatus.h"

MariaUIStatus::MariaUIStatus(QMainWindow *parent) {
	_parent=parent;

	loadImages();

	_statusImageIndex=0;
	_statusIcon = new QLabel(parent);
	_statusIcon->setPixmap(*_imageHandle[_statusImageIndex]);
	_statusIcon->hide();

	_statusAnimationTimer = new QTimer(this);
    connect(_statusAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStatusAnimation()));

}

MariaUIStatus::~MariaUIStatus(void) {
	for(int i=0;i<AMOUNT_OF_IMAGES;i++) {
		delete _imageHandle[i];
	}
}

void MariaUIStatus::loadImages() {
	_imageHandle[0] = new QPixmap("./Resources/ui_status_recognise.png");
	_imageHandle[1] = new QPixmap("./Resources/ui_status_unknown.png");
	_imageHandle[2] = new QPixmap("./Resources/ui_status_wait_0.png");
	_imageHandle[3] = new QPixmap("./Resources/ui_status_wait_1.png");
	_imageHandle[4] = new QPixmap("./Resources/ui_status_wait_2.png");
	_imageHandle[5] = new QPixmap("./Resources/ui_status_wait_3.png");
}

void MariaUIStatus::updateStatusAnimation() {
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

void MariaUIStatus::setStatus(STATUS_TYPE type) {
	if(_currentStatus!=type) {
		_currentStatus=type;
		updateStatusAnimation();
		if(!_statusAnimationTimer->isActive())
			_statusAnimationTimer->start(500);
	}
}

MariaUIStatus::STATUS_TYPE MariaUIStatus::getStatus() {
	return _currentStatus;
}

void MariaUIStatus::updateGUI(float rollingX, float rollingY) {
	_statusIcon->setGeometry(QRect(rollingX, rollingY, 20, 20));
}