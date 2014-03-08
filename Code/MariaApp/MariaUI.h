#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qtoolbutton.h>
#include "MariaUICommandBar.h"
#include "MariaUITextbox.h"

class MariaLogic;
class MariaUI : QMainWindow {
	 Q_OBJECT

	static const int WINDOW_DEFAULT_SIZE_X=480;
	static const int WINDOW_DEFAULT_SIZE_Y=160;
	static const int WINDOW_DEFAULT_EXPAND_SIZE_X=480;
	static const int WINDOW_DEFAULT_EXPAND_SIZE_Y=360;
	static const float CLOSE_BUTTON_X_OFFSET;
	static const float CLOSE_BUTTON_Y_OFFSET;

private:
	MariaLogic *_mariaLogic;
	
	MariaUICommandBar* _commandBar;

	QToolButton *_btClose;
	QString _backgroundColor;
	bool _expandView;

	//Load images used in application.
	void initWindow();
	void initButtons();

public slots:
	void quitAction();

protected:
	void resizeEvent(QResizeEvent *event);
	void keyReleaseEvent(QKeyEvent* keyevent);

public:
	MariaUI(MariaLogic *mariaLogic, QWidget *parent = 0);
	~MariaUI(void);

	void setExpand(bool value);
	bool getExpand();
	void setBackgroundColor(const QString text);

	MariaUICommandBar* getCommandBar();
};
