#include <assert.h>
#include "MariaMacros.h"
#include "MariaUIStatus.h"

MariaUIStatus::MariaUIStatus(QMainWindow * qmainWindow) {
	assert(qmainWindow != NULL);
	
	_qmainWindow = qmainWindow;

	loadImages();

	_statusImageIndex = 0;
	_statusIcon = new QLabel(_qmainWindow);
	_statusIcon->setPixmap(*_imageHandle[_statusImageIndex]);
	_statusIcon->hide();

	_statusAnimationTimer = new QTimer(this);
	connect(_statusAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStatusAnimation()));
}

MariaUIStatus::~MariaUIStatus(void) {
	for( int i = 0 ; i < AMOUNT_OF_IMAGES;i++ ) {
		SAFE_DELETE(_imageHandle[i]);
	}
	SAFE_DELETE(_statusAnimationTimer);
	SAFE_DELETE(_statusIcon);
}

void MariaUIStatus::setStatus(STATUS_TYPE type) {
	if(_currentStatus != type) {
		_currentStatus = type;
		updateStatusAnimation();
		if(!_statusAnimationTimer->isActive())
			_statusAnimationTimer->start(ANIMATION_SPEED);
	}
}

MariaUIStatus::STATUS_TYPE MariaUIStatus::getStatus() {
	return _currentStatus;
}

void MariaUIStatus::updateGUI(QPointF statePosition) {
	_statusIcon->setGeometry(QRect(statePosition.x() + TEXTBOX_X_OFFSET, statePosition.y(), _imageHandle[0]->width(), _imageHandle[0]->height()));
}

void MariaUIStatus::updateStatusAnimation() {
	_statusIcon->show();

	switch(_currentStatus) {
	case OK:
		_statusAnimationTimer->stop();
		_statusImageIndex = IMAGE_INDEX_OK;
		break;
	case INVALID:
		_statusAnimationTimer->stop();
		break;
	case WAIT:
		if(_statusImageIndex<IMAGE_INDEX_WAIT_0||_statusImageIndex>IMAGE_INDEX_WAIT_3)
			_statusImageIndex = IMAGE_INDEX_WAIT_0;
		if(_statusImageIndex<IMAGE_INDEX_WAIT_3)
			_statusImageIndex++ ;
		else
			_statusImageIndex = IMAGE_INDEX_WAIT_0;
		break;
	case UNKNOWN:
		_statusAnimationTimer->stop();
		_statusImageIndex = IMAGE_INDEX_UNKNOWN;
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

void MariaUIStatus::loadImages() {
	_imageHandle[IMAGE_INDEX_OK] = new QPixmap("./Resources/ui_status_recognise.png");
	_imageHandle[IMAGE_INDEX_UNKNOWN] = new QPixmap("./Resources/ui_status_unknown.png");
	_imageHandle[IMAGE_INDEX_WAIT_0] = new QPixmap("./Resources/ui_status_wait_0.png");
	_imageHandle[IMAGE_INDEX_WAIT_1] = new QPixmap("./Resources/ui_status_wait_1.png");
	_imageHandle[IMAGE_INDEX_WAIT_2] = new QPixmap("./Resources/ui_status_wait_2.png");
	_imageHandle[IMAGE_INDEX_WAIT_3] = new QPixmap("./Resources/ui_status_wait_3.png");
}
