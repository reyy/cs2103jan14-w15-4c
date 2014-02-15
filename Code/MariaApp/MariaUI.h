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

class MariaUI :	public QMainWindow
{
public:
	QLineEdit *inputBox;

	MariaUI(void);
	~MariaUI(void);
    
private:
	void setupUI();
    void initWindowTitle();
	void initTextBox();
};

