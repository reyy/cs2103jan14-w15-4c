#include "MariaUILoading.h"


MariaUILoading::MariaUILoading(QMainWindow *parent) {
	
	_parent=parent;
	_imageHandleLogo = new QPixmap("./Resources/ui_maria_logo.png");
	_imageHandleDots = new QPixmap("./Resources/ui_loadingdots.png");

	_displayText = new QLabel(_parent);
	_displayText->setStyleSheet("color:#ffffff;");
	_displayText->setAlignment(Qt::AlignCenter);

	_logoYPos=-LOGO_YOFFSET;
	_logo = new QLabel(_parent);
	_logo->setPixmap(*_imageHandleLogo);
	_logo->setAlignment(Qt::AlignCenter);

	_currentState=BEFORE;

	for(int i=0;i<AMOUNT_OF_DOTS;i++) {
		_dotsXPos[i]=_parent->width()+DOTS_XOFFSET+DOTS_XSEPARATE*i;
		_loadingDots[i] = new QLabel(_parent);
		_loadingDots[i]->setPixmap(*_imageHandleDots);
		_loadingDots[i]->setStyleSheet("");
	}

	_loadingAnimationTimer = new QTimer(this);
    connect(_loadingAnimationTimer, SIGNAL(timeout()), this, SLOT(updateStateAnimation()));
	
	updateGUI();
}

MariaUILoading::~MariaUILoading() {
	delete _displayText;
	delete _logo;
	for(int i=0;i<AMOUNT_OF_DOTS;i++) {
		delete _loadingDots[i];
	}
	delete _imageHandleLogo;
	delete _imageHandleDots;
	delete _loadingAnimationTimer;
}

void MariaUILoading::updateStateAnimation() {
	switch(_currentState) {
	case BEFORE:
		if(abs(_logoYPos-_parent->height()*0.4)>0.5) {
			_logoYPos+=(_parent->height()*0.4-_logoYPos)*0.01;
		} else {
			_currentState = DURING;
			_logoYPos=_parent->height()*0.4;
			_displayText->show();
			for(int i=0;i<AMOUNT_OF_DOTS;i++) {
				_loadingDots[i]->show();
			}
		}
		break;
	case DURING:
		for(int i=0;i<AMOUNT_OF_DOTS;i++) {
			if(_dotsXPos[i]<-DOTS_XOFFSET) {
				_dotsXPos[i]=_parent->width()+DOTS_XOFFSET;

				if(_toEndAnimation) {
					_displayText->hide();
					_currentState=AFTER;
				}
			} else {
				float newSpeed=_dotsXPos[i]/_parent->width();
				_dotsXPos[i]-=abs(newSpeed-0.5)*0.5+(float)(DOTS_XSPEED*0.1);
			}
		}
		break;
	case AFTER:
		//Clean any remaining flying dots.
		for(int i=0;i<AMOUNT_OF_DOTS;i++) {
			_loadingDots[i]->hide();
		}

		if(abs(_logoYPos-(-LOGO_YOFFSET))>1.0) {
			_logoYPos+=((-LOGO_YOFFSET)-_logoYPos)*0.01;
		} else {
			_currentState = DONE;
			_logoYPos=-LOGO_YOFFSET;
		}
		break;
	case DONE:
		_loadingAnimationTimer->stop();
		break;
	default:
		break;
	}
	updateGUI();
}

void MariaUILoading::updateGUI() {
	_logo->setGeometry(QRect(_parent->width()*0.5-60, _logoYPos-35, 120,60));
	_displayText->setGeometry(QRect(_parent->width()*0.5-150, _logoYPos+25, 300,20));
	
	for(int i=0;i<AMOUNT_OF_DOTS;i++) {
		_loadingDots[i]->setGeometry(QRect((int)(_dotsXPos[i]),(int)(_parent->height()*0.8),8,8));
	}
}

bool MariaUILoading::isAnimationDone() {
	if(_currentState==DONE) {
		return true;
	} else {
		return false;
	}
}

void MariaUILoading::startLoadingAnimation() {

	if(!_loadingAnimationTimer->isActive()) {
		for(int i=0;i<AMOUNT_OF_DOTS;i++) {
			_dotsXPos[i]=_parent->width()+DOTS_XOFFSET+DOTS_XSEPARATE*i;
		}
		_loadingAnimationTimer->start(1);
		_currentState=BEFORE;
		_displayText->hide();
		_toEndAnimation=false;
	}
}

void MariaUILoading::endLoadingAnimation() {
	_toEndAnimation=true;
}

void MariaUILoading::setDisplayText(const QString text) {
	_displayText->setText(text);
}