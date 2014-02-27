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
#include "MariaUILoading.h"

#define WINDOW_DEFAULT_SIZE_X 480
#define WINDOW_DEFAULT_SIZE_Y 120

#define AMOUNT_OF_IMAGES 10

class MariaUI : QMainWindow {
	 Q_OBJECT
public:
	enum STATUS_TYPE {
		OK, INVALID, WAIT, UNKNOWN, NONE
	};
	enum STATE_TYPE {
		DEFAULT, FOCUS, INTRO
	};
private:
	int _statusImageIndex;				//The actual index for QLabel to reference to QPixmap. Change to switch image.
	QTimer *_statusAnimationTimer;
	QTimer *_stateAnimationTimer;
	QLineEdit *_inputBox;
	QPixmap **_imageHandle;
	QLabel *_statusIcon;
	QLabel *_suggestText;
	QLabel *_questionText;
	STATUS_TYPE _currentStatus;
	STATE_TYPE _currentState;
	QToolButton *_btClose;
	QPointF _toolBoxCoordinate;
	MariaUILoading *_mariaUILoading;
	QString _backgroundColor;

	//Load images used in application.
	void initState();
	void initImages();
	void initWindowTitle();
	void initTextBox();
	void initStatusIcon();
	void initButtons();
	void initLoading();

private slots:
	void updateStatusAnimation();
	void updateStateAnimation();

protected:
	void resizeEvent(QResizeEvent *event);

public:

	MariaUI(QWidget *parent=0);
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

	void setBackgroundColor(const QString text);

	void beginLoading();
	void endLoading();
};
