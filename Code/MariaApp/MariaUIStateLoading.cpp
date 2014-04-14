//@author A0111784H
#include <assert.h>
#include "MariaMacros.h"
#include "MariaUIStateLoading.h"
#include "MariaUI.h"

const float MariaUIStateLoading::FLOW_FACTOR = 0.01;
const float MariaUIStateLoading::VALUE_THRESHOLD = 0.5;
const float MariaUIStateLoading::DISPLAY_TEXT_WIDTH = 300;
const float MariaUIStateLoading::DISPLAY_TEXT_HEIGHT = 20;
const float MariaUIStateLoading::LOGO_START_Y = -100;
const float MariaUIStateLoading::LOGO_STAGE_Y_SCALE = 0.4;
const float MariaUIStateLoading::TEXT_STAGE_Y_SCALE = 0.6;
const float MariaUIStateLoading::DOTS_STAGE_Y_SCALE = 0.8;
const float MariaUIStateLoading::DOTS_X_OFFSET = 10;
const float MariaUIStateLoading::DOTS_SEPARATION_WIDTH = 50;
const float MariaUIStateLoading::DOTS_X_SPEED = 0.15;
const float MariaUIStateLoading::DOTS_X_VARIABLE_SPEED = 0.75;

MariaUIStateLoading::MariaUIStateLoading(QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	assert(qmainWindow != NULL);

	_qmainWindow = qmainWindow;
	_displayText = new QLabel(_qmainWindow);
	_logo = new QLabel(_qmainWindow);

	for (int i = 0; i < AMOUNT_OF_DOTS; i++) {
		_loadingDots[i] = new QLabel(_qmainWindow);
	}

	_logoImageIndex = 0;
	_quitAfterLoading = false;
	_doneLoading = false;
	_transitionAuto = true;
}

MariaUIStateLoading::~MariaUIStateLoading() {
	for (int i = 0; i < AMOUNT_OF_DOTS; i++) {
		SAFE_DELETE(_loadingDots[i]);
	}
	SAFE_DELETE(_logo);
	SAFE_DELETE(_displayText);
}

void MariaUIStateLoading::setDisplayText(string text) {
	QString qText = QString::fromStdString(text);
	_displayText->setText(qText);
}

void MariaUIStateLoading::setLoadingDone() {
	_doneLoading = true;
}

void MariaUIStateLoading::setQuitAfterLoadingTrue() {
	_quitAfterLoading = true;
}

void MariaUIStateLoading::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);

	((MariaUI*)_qmainWindow)->setBackgroundColor(MariaUI::WINDOW_DEFAULT_COLOR_R, MariaUI::WINDOW_DEFAULT_COLOR_G, MariaUI::WINDOW_DEFAULT_COLOR_B);

	_displayText->setStyleSheet("color:#ffffff;");
	_displayText->setAlignment(Qt::AlignCenter);
	_displayText->setGeometry(QRect(_qmainWindow->width() * 0.5 - DISPLAY_TEXT_WIDTH * 0.5, _qmainWindow->height() * TEXT_STAGE_Y_SCALE, DISPLAY_TEXT_WIDTH, DISPLAY_TEXT_HEIGHT));
	_displayText->hide();

	_logo->setPixmap(*MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON));
	_logo->setAlignment(Qt::AlignCenter);
	_logoYPos = LOGO_START_Y;
	_logo->setGeometry(QRect(_qmainWindow->width() * 0.5 - MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->width() * 0.5, _logoYPos, MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->width(), MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->height()));
	_logo->show();
}

void MariaUIStateLoading::initActiveState() {
	_displayText->show();
	
	for (int i = 0; i < AMOUNT_OF_DOTS; i++) {
		_loadingDots[i]->setPixmap(*MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_DOTS));
		_loadingDots[i]->show();
		_dotsXPos[i] = _qmainWindow->width() + DOTS_X_OFFSET + DOTS_SEPARATION_WIDTH * i;
	}
}

void MariaUIStateLoading::initEndState() {
	_displayText->hide();

	for (int i = 0; i < AMOUNT_OF_DOTS; i++) {
		_loadingDots[i]->hide();
	}
}

bool MariaUIStateLoading::timerBeginState() {
	animateLogo();

	if (abs(_logoYPos -_qmainWindow->height() * LOGO_STAGE_Y_SCALE)>VALUE_THRESHOLD) {
		_logoYPos += (_qmainWindow->height() * LOGO_STAGE_Y_SCALE - _logoYPos) * FLOW_FACTOR;
		updateGUIPosition();

		return true;
	}
	return false;
}

bool MariaUIStateLoading::timerActiveState() {
	animateLogo();
	bool allDotsLeft = animateDots();

	if (allDotsLeft && _doneLoading) {
		return false;
	} else {
		return true;
	}
}

bool MariaUIStateLoading::timerEndState() {
	animateLogo();

	if (abs(_logoYPos - LOGO_START_Y)>VALUE_THRESHOLD) {
		_logoYPos += (LOGO_START_Y - _logoYPos) * FLOW_FACTOR;
		updateGUIPosition();
		
		return true;
	} else {
		if (_quitAfterLoading) {
			((MariaUI*)_qmainWindow)->quitAction();
		}

		return false;
	}
}

void MariaUIStateLoading::animateLogo() {
	if (_logoImageIndex + LOGO_SPEED_RETARDER < MariaUI::AMOUNT_OF_ICON * LOGO_SPEED_RETARDER) {
		_logoImageIndex++;
	} else {
		_logoImageIndex = 0;
	}
	_logo->setPixmap(*MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON + _logoImageIndex / LOGO_SPEED_RETARDER));
}

bool MariaUIStateLoading::animateDots() {
	bool allDotsLeft = true;
	for (int i = 0; i < AMOUNT_OF_DOTS; i++) {
		if (_dotsXPos[i]< - DOTS_X_OFFSET - AMOUNT_OF_DOTS * DOTS_SEPARATION_WIDTH) {
			_dotsXPos[i] = _qmainWindow->width() + DOTS_X_OFFSET;
		} else {
			float newSpeed = (_dotsXPos[i] -_qmainWindow->width() * 0.5) / _qmainWindow->width();
			_dotsXPos[i] -= abs(newSpeed) * DOTS_X_VARIABLE_SPEED + DOTS_X_SPEED;
		}

		if (_dotsXPos[i]> - DOTS_X_OFFSET) {
			allDotsLeft = false;
		}

		_loadingDots[i]->setGeometry(QRect(_dotsXPos[i] - MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_DOTS)->width() * 0.5, _qmainWindow->height() * DOTS_STAGE_Y_SCALE , MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_DOTS)->width(), MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_DOTS)->height()));
	}

	return allDotsLeft;
}

void MariaUIStateLoading::updateGUIPosition() {
	_logo->setGeometry(QRect(_qmainWindow->width() * 0.5 - MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->width() * 0.5, _logoYPos - MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->height() * 0.5, MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->width(), MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->height()));
}