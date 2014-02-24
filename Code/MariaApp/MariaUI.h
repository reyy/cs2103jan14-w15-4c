#pragma once
#include <QtWidgets/QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QlineEdit>

#define WINDOW_SIZE_X 480
#define WINDOW_SIZE_Y 120

class MariaUI : QMainWindow
{
private:

	QLineEdit *inputBox;

    void initWindowTitle();
	void initTextBox();

public:
	MariaUI();
	~MariaUI(void);
    
};

