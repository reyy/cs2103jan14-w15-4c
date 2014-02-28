#pragma once

#include <QtWidgets/QMainWindow>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QlineEdit>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>
#include <QtWidgets/qtoolbutton.h>
#include <queue>
#include "MariaUILoading.h"

#define WINDOW_DEFAULT_SIZE_X 480
#define WINDOW_DEFAULT_SIZE_Y 120

#define WINDOW_DEFAULT_EXPAND_SIZE_X 480
#define WINDOW_DEFAULT_EXPAND_SIZE_Y 360

#define AMOUNT_OF_IMAGES 7

class MariaLogic;
class MariaUI : QMainWindow {
	 Q_OBJECT
public:
	enum STATUS_TYPE {
		OK, INVALID, WAIT, UNKNOWN, NONE
	};
	enum STATE_TYPE {
		DEFAULT, FOCUS, INTRO, QUIT
	};
private:
	MariaLogic *_mariaLogic;

	QTimer *_statusAnimationTimer;
	QTimer *_statePreAnimationTimer;
	QTimer *_statePosAnimationTimer;
	
	QPixmap **_imageHandle;
	QLabel *_statusIcon;
	int _statusImageIndex;				//The actual index for QLabel to reference to QPixmap. Change to switch image.
	STATUS_TYPE _currentStatus;

	QLabel *_questionText;
	QLineEdit *_inputBox;
	QLabel *_suggestText;
	
	STATE_TYPE _currentState;
	std::queue<STATE_TYPE> _stateQueue;		//The state to transit into.
	bool _processingState;
	QPointF _toolBoxCoordinate;
	float _stateTargetY;
	
	QToolButton *_btClose;
	MariaUILoading *_loading;
	QString _backgroundColor;
	bool _expandView;

	//Load images used in application.
	void initState();
	void initImages();
	void initWindow();
	void initTextBox();
	void initStatusIcon();
	void initButtons();
	void initLoading();

private slots:
	void updateStatusAnimation();
	void updateStatePreAnimation();
	void updateStatePosAnimation();

protected:
	void resizeEvent(QResizeEvent *event);
	void keyReleaseEvent(QKeyEvent* keyevent);
	void beginNewState();
	void endOldState();

public:

	MariaUI(MariaLogic *mariaLogic, QWidget *parent=0);
	~MariaUI(void);

	//Force UI Class to update values prematurelly.
	void updateGUI();

	void setStatus(STATUS_TYPE type);
	STATUS_TYPE getStatus();

	void setState(STATE_TYPE type);
	STATE_TYPE getState();

	void setBaseText(const QString text);
	void setQuestionText(const QString text);
	
	QString getUserInput();
	void setUserInput(const QString text);

	void setExpand(bool value);
	bool getExpand();

	void setBackgroundColor(const QString text);

	MariaUILoading* getLoading();
};
