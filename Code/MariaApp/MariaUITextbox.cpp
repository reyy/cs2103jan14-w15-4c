#include "MariaUITextbox.h"


MariaUITextbox::MariaUITextbox(QMainWindow *parent) {
	_parent = parent;

	_inputBox = new QLineEdit(_parent);
	_inputBox->setStyleSheet("background-color: #ffffff;border:0px;border-radius: 5px;");

	_suggestText = new QLabel(_parent);
	_suggestText->setStyleSheet("background-color: rgba(0,0,0,0);border:0px;");	

	_questionText = new QLabel(_parent);
	_questionText->setStyleSheet("background-color: rgba(0,0,0,0);border:0px;color:#ffffff;font-weight:bold;");	
}

MariaUITextbox::~MariaUITextbox() {
	delete _inputBox;
	delete _suggestText;
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

void MariaUITextbox::updateGUI(float rollingX, float rollingY) {
	_inputBox->setGeometry(QRect(rollingX+30,rollingY, _parent->width()-rollingX-60, 20));
	_suggestText->setGeometry(QRect(rollingX+33, rollingY, _parent->width()-rollingX-60, 20));
	_questionText->setGeometry(QRect(rollingX+33, rollingY-20, _parent->width()-rollingX-60, 20));
}