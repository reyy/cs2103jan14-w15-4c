#pragma once

#include <string>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QlineEdit>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>

using namespace std;

class MariaUITextbox : QWidget{
	Q_OBJECT

public:
	static const int SUGGEST_TEXT_AMOUNT = 8;
	static const int SUGGEST_TEXT_UPDATE_TIME = 5000;
	static const int RESERVED_STATUS_SPACE = 30;
	static const int TEXTBOX_X_OFFSET = 30;
	static const int TEXTBOX_HEIGHT = 20;
	static const int QUESTION_TEXT_X_OFFSET = 3;
	static const int QUESTION_TEXT_Y_OFFSET = -52;
	static const int QUESTIONBOX_HEIGHT = 48;
	static const string SUGGEST_TEXT_SAMPLE[];
	static const float FONT_SIZE_TYPED_TEXT;
	static const float FONT_SIZE_QUESTION_TEXT;

	MariaUITextbox(QMainWindow * qmainWindow);
	~MariaUITextbox();
	
	void setQuestionText(const string text);
	void setSuggestText(const string text);
	void setUserInput(const string text);	
	string getUserInput();
	void updateGUI(QPointF statePosition);
	void setFocus();
	QLineEdit * getInputBoxReference();

private:
	QMainWindow * _qmainWindow;

	QLineEdit *_inputBox;
	QLabel *_questionText;
	QLabel *_suggestText;

	QTimer *_updateSuggestTextTimer;

private slots:
	void updateSuggestText();
};

