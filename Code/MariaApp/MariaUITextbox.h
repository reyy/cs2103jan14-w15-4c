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
	static const string SUGGEST_TEXT_SAMPLE[];
	static const int SUGGEST_TEXT_AMOUNT = 8;

	static const int SUGGEST_TEXT_UPDATE_TIME = 5000;

	static const int RESERVED_STATUS_SPACE = 30;
	static const int TEXTBOX_X_OFFSET = 30;
	static const int TEXTBOX_HEIGHT = 20;
	static const int QUESTION_TEXT_X_OFFSET = 3;
	static const int QUESTION_TEXT_Y_OFFSET = -52;
	static const int QUESTIONBOX_HEIGHT = 48;
	static const float FONT_SIZE_TYPED_TEXT;
	static const float FONT_SIZE_QUESTION_TEXT;

private:
	QMainWindow * _qmainWindow;

	QLabel *_questionText;
	QLineEdit *_inputBox;
	QLabel *_suggestText;

	QTimer *_updateSuggestTextTimer;

private slots:
	void updateSuggestText();

public:
	MariaUITextbox(QMainWindow * qmainWindow);
	~MariaUITextbox();
	
	void setQuestionText(const std::string text);
	void setSuggestText(const std::string text);
	void setUserInput(const std::string text);	
	std::string getUserInput();
	void updateGUI(QPointF statePosition);
	void setFocus();
	QLineEdit * getInputBoxReference();
};

