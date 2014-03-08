#include "MariaUITextbox.h"


MariaUITextbox::MariaUITextbox(QMainWindow *qmainWindow) {
	_qmainWindow = qmainWindow;

	_inputBox = new QLineEdit(_qmainWindow);
	_inputBox->setStyleSheet("background-color: #ffffff;border:0px;border-radius: 5px;color:#000000;");

	_suggestText = new QLabel(_qmainWindow);
	_suggestText->setStyleSheet("background-color: rgba(0,0,0,0);border:0px;color:#111111;");	

	_questionText = new QLabel(_qmainWindow);
	_questionText->setStyleSheet("background-color: rgba(0,0,0,0);border:0px;color:#ffffff;font-weight:bold;");	
}

MariaUITextbox::~MariaUITextbox() {
	delete _suggestText;
	delete _inputBox;
	delete _questionText;
}

void MariaUITextbox::setQuestionText(const std::string text) {
	_questionText->setText(QString::fromStdString(text));
}

void MariaUITextbox::setSuggestText(const std::string text) {
	_suggestText->setText(QString::fromStdString(text));
}

void MariaUITextbox::setUserInput(const std::string text) {
	_inputBox->setText(QString::fromStdString(text));
}
	
std::string MariaUITextbox::getUserInput() {
	return _inputBox->text().toStdString();
}

void MariaUITextbox::updateGUI(QPointF statePosition) {
	_inputBox->setGeometry(QRect(statePosition.x()+RESERVED_STATUS_SPACE+TEXTBOX_X_OFFSET,statePosition.y(), _qmainWindow->width()-TEXTBOX_X_OFFSET*2-RESERVED_STATUS_SPACE, TEXTBOX_HEIGHT));
	_suggestText->setGeometry(QRect(statePosition.x()+RESERVED_STATUS_SPACE+TEXTBOX_X_OFFSET+QUESTION_TEXT_X_OFFSET,statePosition.y(), _qmainWindow->width()-TEXTBOX_X_OFFSET*2-RESERVED_STATUS_SPACE, TEXTBOX_HEIGHT));
	_questionText->setGeometry(QRect(statePosition.x()+RESERVED_STATUS_SPACE+TEXTBOX_X_OFFSET+QUESTION_TEXT_X_OFFSET,statePosition.y()+QUESTION_TEXT_Y_OFFSET, _qmainWindow->width()-TEXTBOX_X_OFFSET*2-RESERVED_STATUS_SPACE, TEXTBOX_HEIGHT));
}

void MariaUITextbox::setFocus() {
	_inputBox->setFocus();
}