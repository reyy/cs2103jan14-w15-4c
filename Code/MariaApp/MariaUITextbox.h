#pragma once
#include <string>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QlineEdit>
#include <QtWidgets/qlabel.h>

class MariaUITextbox {
	QMainWindow *_parent;

	QLabel *_questionText;
	QLineEdit *_inputBox;
	QLabel *_suggestText;

public:
	MariaUITextbox(QMainWindow *parent);
	~MariaUITextbox();
	
	void setQuestionText(const std::string text);
	void setSuggestText(const std::string text);
	void setUserInput(const std::string text);	
	std::string getUserInput();
	void updateGUI(float rollingX, float rollingY);
};

