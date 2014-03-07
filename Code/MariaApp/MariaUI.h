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
#include <queue>
#include "MariaUILoading.h"
#include "MariaUICalendar.h"
#include "MariaUIHome.h"
#include "MariaUIStatus.h"
#include "MariaUITextbox.h"

#define WINDOW_DEFAULT_SIZE_X 480
#define WINDOW_DEFAULT_SIZE_Y 160

#define WINDOW_DEFAULT_EXPAND_SIZE_X 480
#define WINDOW_DEFAULT_EXPAND_SIZE_Y 360


class MariaLogic;
class MariaTaskManager;
class MariaUI : QMainWindow {
	 Q_OBJECT
public:
	enum STATE_TYPE {
		DEFAULT, HOME, FOCUS_CALENDAR,FOCUS_SETTING, INTRO, QUIT
	};
private:
	MariaLogic *_mariaLogic;
	MariaTaskManager *_mariaTaskManager;

	MariaUILoading *_loading;
	MariaUICalendar *_calendar;
	MariaUIHome *_home;
	MariaUIStatus *_status;
	MariaUITextbox *_textbox;
	
	STATE_TYPE _currentState;
	std::queue<STATE_TYPE> _stateQueue;		//The state to transit into.
	QPointF _rollingCoord;
	QPointF _destCoord;
	bool _processingState;
	QTimer *_statePreAnimationTimer;
	QTimer *_statePosAnimationTimer;
	
	QToolButton *_btClose;
	QString _backgroundColor;
	bool _expandView;

	//Load images used in application.
	void initWindow();
	void initState();
	void initSubclasses();
	void initButtons();

private slots:
	void updateStatePreAnimation();
	void updateStatePosAnimation();
	void quitAction();

protected:
	void resizeEvent(QResizeEvent *event);
	void keyReleaseEvent(QKeyEvent* keyevent);
	void beginNewState();
	void endOldState();

public:

	MariaUI(MariaLogic *mariaLogic,MariaTaskManager *mariaTaskManager, QWidget *parent=0);
	~MariaUI(void);

	//Force UI Class to update values prematurelly.
	void updateGUI();

	void setState(STATE_TYPE type);
	STATE_TYPE getState();
	void setExpand(bool value);
	bool getExpand();
	void setBackgroundColor(const QString text);
	
	MariaUILoading* getLoading();
	MariaUICalendar* getCalendar();
	MariaUIHome* getHome();
	MariaUIStatus* getStatus();
	MariaUITextbox* getTextbox();
};
