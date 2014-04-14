//@author A0111784H
#include <assert.h>
#include "MariaMacros.h"
#include "MariaUI.h"
#include "MariaUIStateCredits.h"

const string MariaUIStateCredits::CREDITS_STRING = "MARIA\nMy Attentive, Reliable and Intelligent Assistant";
const string MariaUIStateCredits::CREDITS_STRING_BODY = "Developed for CS2103T Software Engineering(S2 2014)\nNational University of Singapore\n\nDevelopers:\nJay Chua\nMelvyn Qwek\nRey Neo\n\nSpecial Thanks:\nDr Damith C. Rajapakse\nMrs Janet Chan\nMr Jerome Cheng";
const float MariaUIStateCredits::FLOW_FACTOR = 0.01;
const float MariaUIStateCredits::VALUE_THRESHOLD = 0.5;
const float MariaUIStateCredits::DISPLAY_TEXT_WIDTH = 400;
const float MariaUIStateCredits::DISPLAY_TEXT_HEIGHT = 60;
const float MariaUIStateCredits::DISPLAYBODY_TEXT_WIDTH = 400;
const float MariaUIStateCredits::DISPLAYBODY_TEXT_HEIGHT = 400;
const float MariaUIStateCredits::LOGO_START_Y = -100;
const float MariaUIStateCredits::LOGO_STAGE_Y_SCALE = 0.15;
const float MariaUIStateCredits::TEXT_STAGE_Y_SCALE = 0.3;
const float MariaUIStateCredits::TEXTBODY_STAGE_Y_SCALE = 0.3;

MariaUIStateCredits::MariaUIStateCredits(QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	assert(qmainWindow != NULL);
	
	_qmainWindow = qmainWindow;
	
	_displayText = new QLabel(_qmainWindow);
	_displayTextBody = new QLabel(_qmainWindow);
	_logo = new QLabel(_qmainWindow);
	_logoImageIndex = 0;
	_doneAnimating = false;
	_transitionAuto = true;
}

MariaUIStateCredits::~MariaUIStateCredits() {
	SAFE_DELETE(_logo);
	SAFE_DELETE(_displayTextBody);
	SAFE_DELETE(_displayText);
}

void MariaUIStateCredits::setDoneAnimating() {
	_doneAnimating = true;
}

//@author generated
void MariaUIStateCredits::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(MariaUICommandBar::DEFAULT_Y_POSITION);

	((MariaUI*)_qmainWindow)->setBackgroundColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);

	_displayText->setStyleSheet("color:#ffffff;font-size:" + QString::number(FONT_SIZE) + "px;font-weight:bold;");
	_displayText->setAlignment(Qt::AlignCenter);
	_displayText->setGeometry(QRect(_qmainWindow->width(), _qmainWindow->height(), 0.0, 0.0));
	_displayText->setText(QString::fromStdString(CREDITS_STRING));
	_displayText->show();

	_displayTextBody->setStyleSheet("color:#ffffff;font-size:" + QString::number(FONT_SIZE_BODY) + "px;");
	_displayTextBody->setAlignment(Qt::AlignCenter);
	_displayTextBody->setGeometry(QRect(_qmainWindow->width(), _qmainWindow->height(), 0.0, 0.0));
	_displayTextBody->setText(QString::fromStdString(CREDITS_STRING_BODY));
	_displayTextBody->show();

	_logo->setPixmap(*MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON));
	_logo->setAlignment(Qt::AlignCenter);
	_logo->setGeometry(QRect(_qmainWindow->width(), _qmainWindow->height(), 0.0, 0.0));
	_logo->show();
}

//@author A0111784H
void MariaUIStateCredits::initActiveState() {
}

void MariaUIStateCredits::initEndState() {
}

bool MariaUIStateCredits::timerBeginState() {
	animateLogo();
	updateGUIPosition();
	return false;
}

bool MariaUIStateCredits::timerActiveState() {
	animateLogo();

	if (_doneAnimating) {
		return false;
	} else {
		return true;
	}
}

bool MariaUIStateCredits::timerEndState() {
	animateLogo();
	updateGUIPosition();
	return false;
}

void MariaUIStateCredits::animateLogo() {
	if (_logoImageIndex + LOGO_SPEED_RETARDER < MariaUI::AMOUNT_OF_ICON * LOGO_SPEED_RETARDER) {
		_logoImageIndex++;
	} else {
		_logoImageIndex = 0;
	}
	_logo->setPixmap(*MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON + _logoImageIndex / LOGO_SPEED_RETARDER));
}

void MariaUIStateCredits::updateGUIPosition() {
	_displayText->setGeometry(QRect(getPosition().x() + _qmainWindow->width() * 0.5 - DISPLAY_TEXT_WIDTH * 0.5, getPosition().y() + _qmainWindow->height() * TEXT_STAGE_Y_SCALE, DISPLAY_TEXT_WIDTH, DISPLAY_TEXT_HEIGHT));
	_displayTextBody->setGeometry(QRect(getPosition().x() + _qmainWindow->width() * 0.5 - DISPLAY_TEXT_WIDTH * 0.5, getPosition().y() + _qmainWindow->height() * TEXTBODY_STAGE_Y_SCALE, DISPLAYBODY_TEXT_WIDTH, DISPLAYBODY_TEXT_HEIGHT));
	_logo->setGeometry(QRect(getPosition().x() + _qmainWindow->width() * 0.5 - MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->width() * 0.5, getPosition().y() + _qmainWindow->height() * LOGO_STAGE_Y_SCALE, MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->width(), MariaUI::getImageHandler(MariaUI::IMAGE_INDEX_ICON)->height()));
}