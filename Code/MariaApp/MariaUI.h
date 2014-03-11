#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtCore/QTimer>
#include <QtWidgets/qtoolbutton.h>
#include <QtWidgets/qsystemtrayicon.h>
#include "MariaUICommandBar.h"
#include "MariaUITextbox.h"
#include <QtWidgets/qshortcut.h>

class MariaLogic;
class MariaUI : QMainWindow {
	 Q_OBJECT

public:
	static const float FLOW_FACTOR;
	static const float VALUE_THRESHOLD;
	static const int WINDOW_DEFAULT_SIZE_X=480;
	static const int WINDOW_DEFAULT_SIZE_Y=320;
	static const int WINDOW_DEFAULT_EXPAND_SIZE_X=480;
	static const int WINDOW_DEFAULT_EXPAND_SIZE_Y=600;
	static const float CLOSE_BUTTON_X_OFFSET;
	static const float CLOSE_BUTTON_Y_OFFSET;
	static const float WINDOW_DEFAULT_COLOR_R;
	static const float WINDOW_DEFAULT_COLOR_G;
	static const float WINDOW_DEFAULT_COLOR_B;

private:
	MariaLogic *_mariaLogic;
	
	MariaUICommandBar* _commandBar;

	QToolButton *_btClose;
	bool _expandView;
	QTimer *_bkgColorUpdateTimer;
	QColor _bkgColor;
	QColor _targetBkgColor;
	QSystemTrayIcon *trayIcon;


	//Load images used in application.
	void initWindow();
	void initButtons();
	void initBackgroundColor(int r, int g, int b);

public slots:
	void quitAction();

protected slots:
	void updateBackgroundColor();
	void showHideEvent();

protected:
	void resizeEvent(QResizeEvent *event);
	void keyReleaseEvent(QKeyEvent* keyevent);

public:
	MariaUI(MariaLogic *mariaLogic, QWidget *parent = 0);
	~MariaUI(void);


	void setExpand(bool value);
	bool getExpand();
	void setBackgroundColor(int r, int g, int b);


	MariaUICommandBar* getCommandBar();

signals:	
	void triggerShowHideEvent();
};
