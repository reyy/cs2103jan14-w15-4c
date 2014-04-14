//@author A0111784H
#include <assert.h>
#include "MariaMacros.h"
#include "MariaUITextbox.h"

const string MariaUITextbox::SUGGEST_TEXT_SAMPLE[] = {
	"create Meeting with mom for dinner soon",
	"create Do my assignment by today 23:59",
	"create Make a trip to the library from today to tomorrow",
	"edit Eat an apple change title Eat a pear",
	"show today",
	"help",
	"delete assignment",
	"quit",
};

//@author generated
const float MariaUITextbox::FONT_SIZE_TYPED_TEXT = 14.0;
const float MariaUITextbox::FONT_SIZE_QUESTION_TEXT = 16.0;

MariaUITextbox::MariaUITextbox(QMainWindow* qmainWindow) {
	assert(qmainWindow != NULL);

	_qmainWindow = qmainWindow;

	_inputBox = new QLineEdit(_qmainWindow);
	_inputBox->setStyleSheet("background-color: #ffffff;border:0px;border-radius: 5px;color:#000000;font-size:" + QString::number(FONT_SIZE_TYPED_TEXT) + "px;");
	_inputBox->installEventFilter(_qmainWindow);

	_suggestText = new QLabel(_qmainWindow);
	_suggestText->setStyleSheet("background-color: rgba(0, 0, 0, 0);border:0px;color:#555555;font-size:" + QString::number(FONT_SIZE_TYPED_TEXT) + "px;");

	_questionText = new QLabel(_qmainWindow);
	_questionText->setStyleSheet("background-color: rgba(0, 0, 0, 0);border:0px;color:#ffffff;font-weight:bold;font-size:" + QString::number(FONT_SIZE_QUESTION_TEXT) + "px;");
	_questionText->setAlignment(Qt::AlignBottom|Qt::AlignLeft);
	_questionText->setWordWrap(true);

	_updateSuggestTextTimer = new QTimer(_qmainWindow);
	connect(_updateSuggestTextTimer, SIGNAL(timeout()), this, SLOT(updateSuggestText()));
	updateSuggestText();
	_updateSuggestTextTimer->start(SUGGEST_TEXT_UPDATE_TIME);
}

MariaUITextbox::~MariaUITextbox() {
	if (_updateSuggestTextTimer->isActive()) {
		_updateSuggestTextTimer->stop();
	}

	SAFE_DELETE(_updateSuggestTextTimer);
	SAFE_DELETE(_suggestText);
	SAFE_DELETE(_inputBox);
	SAFE_DELETE(_questionText);
}

//@author A0111784H
void MariaUITextbox::setQuestionText(const string text) {
	QFontMetrics fm = QFontMetrics(_questionText->font());
	_questionText->setText(fm.elidedText(QString::fromStdString(text), Qt::ElideRight, _questionText->width()));
}

void MariaUITextbox::setSuggestText(const string text) {
	QFontMetrics fm = QFontMetrics(_suggestText->font());
	_suggestText->setText(fm.elidedText(QString::fromStdString(text), Qt::ElideRight, _suggestText->width()));
}

void MariaUITextbox::setUserInput(const string text) {
	QFontMetrics fm = QFontMetrics(_inputBox->font());
	_inputBox->setText(fm.elidedText(QString::fromStdString(text), Qt::ElideRight, _inputBox->width()));
}
	
string MariaUITextbox::getUserInput() {
	return _inputBox->text().toStdString();
}

void MariaUITextbox::updateGUI(QPointF statePosition) {
	_inputBox->setGeometry(QRect(statePosition.x() + RESERVED_STATUS_SPACE + TEXTBOX_X_OFFSET, statePosition.y(), _qmainWindow->width() - TEXTBOX_X_OFFSET - TEXTBOX_X_OFFSET - RESERVED_STATUS_SPACE, TEXTBOX_HEIGHT));
	_suggestText->setGeometry(QRect(statePosition.x() + RESERVED_STATUS_SPACE + TEXTBOX_X_OFFSET + QUESTION_TEXT_X_OFFSET, statePosition.y(), _qmainWindow->width() - TEXTBOX_X_OFFSET - TEXTBOX_X_OFFSET - RESERVED_STATUS_SPACE, TEXTBOX_HEIGHT));
	_questionText->setGeometry(QRect(statePosition.x() + RESERVED_STATUS_SPACE + TEXTBOX_X_OFFSET + QUESTION_TEXT_X_OFFSET, statePosition.y() + QUESTION_TEXT_Y_OFFSET, _qmainWindow->width() - TEXTBOX_X_OFFSET - TEXTBOX_X_OFFSET - RESERVED_STATUS_SPACE, QUESTIONBOX_HEIGHT));
}

void MariaUITextbox::setFocus() {
	_inputBox->setFocus();
}

QLineEdit* MariaUITextbox::getInputBoxReference() {
	return _inputBox;
}

void MariaUITextbox::updateSuggestText() {
	if (getUserInput().length() == 0) {
		setSuggestText(SUGGEST_TEXT_SAMPLE[rand() % SUGGEST_TEXT_AMOUNT]);
	}
}