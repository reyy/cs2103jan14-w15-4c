#pragma once
#include <string>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QlineEdit>
#include <QtWidgets/qlabel.h>

class MariaUITextbox {
	
	static const int RESERVED_STATUS_SPACE=30;
	static const int TEXTBOX_X_OFFSET=30;
	static const int TEXTBOX_HEIGHT=20;
	static const int QUESTION_TEXT_X_OFFSET=3;
	static const int QUESTION_TEXT_Y_OFFSET=-20;

	QMainWindow * _qmainWindow;

	QLabel *_questionText;
	QLineEdit *_inputBox;
	QLabel *_suggestText;

public:
	MariaUITextbox(QMainWindow * qmainWindow);
	~MariaUITextbox();
	
	void setQuestionText(const std::string text);
	void setSuggestText(const std::string text);
	void setUserInput(const std::string text);	
	std::string getUserInput();
	void updateGUI(QPointF statePosition);
	void setFocus();
};

