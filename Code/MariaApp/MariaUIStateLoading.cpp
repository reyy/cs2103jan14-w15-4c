#include "MariaUIStateLoading.h"
#include "MariaUI.h"

const float MariaUIStateLoading::FLOW_FACTOR=0.01;
const float MariaUIStateLoading::VALUE_THRESHOLD=0.5;
const float MariaUIStateLoading::DISPLAY_TEXT_WIDTH=300;
const float MariaUIStateLoading::DISPLAY_TEXT_HEIGHT=20;
const float MariaUIStateLoading::LOGO_START_Y=-50;
const float MariaUIStateLoading::LOGO_STAGE_Y_SCALE=0.4;
const float MariaUIStateLoading::TEXT_STAGE_Y_SCALE=0.6;
const float MariaUIStateLoading::DOTS_STAGE_Y_SCALE=0.8;
const float MariaUIStateLoading::DOTS_X_OFFSET=10;
const float MariaUIStateLoading::DOTS_SEPARATION_WIDTH=50;
const float MariaUIStateLoading::DOTS_X_SPEED=0.15;
const float MariaUIStateLoading::DOTS_X_VARIABLE_SPEED=0.5;

MariaUIStateLoading::MariaUIStateLoading(QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
	_imageHandleLogo = new QPixmap("./Resources/ui_maria_logo.png");
	_imageHandleDots = new QPixmap("./Resources/ui_loadingdots.png");
	_displayText = new QLabel(_qmainWindow);
	_logo = new QLabel(_qmainWindow);
	for(int i=0;i<AMOUNT_OF_DOTS;i++) {
		_loadingDots[i] = new QLabel(_qmainWindow);
	}
	_doneLoading=false;
	_transitionAuto=true;
	_quitAfterLoading=false;
}

MariaUIStateLoading::~MariaUIStateLoading() {
	for(int i=0;i<AMOUNT_OF_DOTS;i++) {
		delete _loadingDots[i];
	}
	delete _logo;
	delete _displayText;
	delete _imageHandleDots;
	delete _imageHandleLogo;
}

void MariaUIStateLoading::initBeginState() {
	((MariaUI*)_qmainWindow)->setBackgroundColor(MariaUI::WINDOW_DEFAULT_COLOR_R,MariaUI::WINDOW_DEFAULT_COLOR_G,MariaUI::WINDOW_DEFAULT_COLOR_B);

	_displayText->setStyleSheet("color:#ffffff;");
	_displayText->setAlignment(Qt::AlignCenter);
	_displayText->setGeometry(QRect(_qmainWindow->width()*0.5-DISPLAY_TEXT_WIDTH*0.5, _qmainWindow->height()*TEXT_STAGE_Y_SCALE, DISPLAY_TEXT_WIDTH,DISPLAY_TEXT_HEIGHT));
	_displayText->hide();

	_logo->setPixmap(*_imageHandleLogo);
	_logo->setAlignment(Qt::AlignCenter);
	_logoYPos=LOGO_START_Y;
	_logo->setGeometry(QRect(_qmainWindow->width()*0.5-_imageHandleLogo->width()*0.5, _logoYPos, _imageHandleLogo->width(),_imageHandleLogo->height()));
	_logo->show();
}

void MariaUIStateLoading::initActiveState() {
	_displayText->show();

	for(int i=0;i<AMOUNT_OF_DOTS;i++) {
		_loadingDots[i]->setPixmap(*_imageHandleDots);
		_loadingDots[i]->show();
		_dotsXPos[i]=_qmainWindow->width()+DOTS_X_OFFSET+DOTS_SEPARATION_WIDTH*i;
	}
}

void MariaUIStateLoading::initEndState() {
	_displayText->hide();

	for(int i=0;i<AMOUNT_OF_DOTS;i++) {
		_loadingDots[i]->hide();
	}
}

bool MariaUIStateLoading::timerBeginState() {
	if(abs(_logoYPos-_qmainWindow->height()*LOGO_STAGE_Y_SCALE)>VALUE_THRESHOLD) {
		_logoYPos+=(_qmainWindow->height()*LOGO_STAGE_Y_SCALE-_logoYPos)*FLOW_FACTOR;
		_logo->setGeometry(QRect(_qmainWindow->width()*0.5-_imageHandleLogo->width()*0.5, _logoYPos-_imageHandleLogo->height()*0.5, _imageHandleLogo->width(),_imageHandleLogo->height()));
		return true;
	}
	return false;
}

bool MariaUIStateLoading::timerActiveState() {
	bool allDotsLeft=true;
	for(int i=0;i<AMOUNT_OF_DOTS;i++) {
		if(_dotsXPos[i]<-DOTS_X_OFFSET-AMOUNT_OF_DOTS*DOTS_SEPARATION_WIDTH) {
			_dotsXPos[i]=_qmainWindow->width()+DOTS_X_OFFSET;
		} else {
			float newSpeed=(_dotsXPos[i]-_qmainWindow->width()*0.5)/_qmainWindow->width();
			_dotsXPos[i]-=abs(newSpeed)*DOTS_X_VARIABLE_SPEED+DOTS_X_SPEED;
		}

		if(_dotsXPos[i]>-DOTS_X_OFFSET) {
			allDotsLeft=false;
		}

		_loadingDots[i]->setGeometry(QRect(_dotsXPos[i]-_imageHandleDots->width()*0.5, _qmainWindow->height()*DOTS_STAGE_Y_SCALE , _imageHandleDots->width(),_imageHandleDots->height()));
	}

	if(allDotsLeft&&_doneLoading) {
		return false;
	} else {
		return true;
	}
}

bool MariaUIStateLoading::timerEndState() {
	if(abs(_logoYPos-LOGO_START_Y)>VALUE_THRESHOLD) {
		_logoYPos+=(LOGO_START_Y-_logoYPos)*FLOW_FACTOR;
		_logo->setGeometry(QRect(_qmainWindow->width()*0.5-_imageHandleLogo->width()*0.5, _logoYPos-_imageHandleLogo->height()*0.5, _imageHandleLogo->width(),_imageHandleLogo->height()));
		if(_quitAfterLoading) {
			((MariaUI*)_qmainWindow)->quitAction();
		}
		return true;
	}
	return false;
}

void MariaUIStateLoading::setDisplayText(const QString text) {
	_displayText->setText(text);
}

void MariaUIStateLoading::setLoadingDone() {
	_doneLoading=true;
}

void MariaUIStateLoading::setQuitAfterLoadingTrue() {
	_quitAfterLoading=true;
}