//@author A0111784H
#pragma once

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qtoolbutton.h>
#include <QtWidgets/qsystemtrayicon.h>
#include <QtWidgets/qshortcut.h>
#include <QtCore/QVariant>
#include <QtCore/QTimer>
#include "MariaUICommandBar.h"
#include "MariaUITextbox.h"

class MariaLogic;
class MariaUI : QMainWindow {
	 Q_OBJECT

public:
	static const int WINDOW_DEFAULT_SIZE_X = 480;
	static const int WINDOW_DEFAULT_SIZE_Y = 640;
	static const float WINDOW_DEFAULT_COLOR_R;
	static const float WINDOW_DEFAULT_COLOR_G;
	static const float WINDOW_DEFAULT_COLOR_B;

	static const int AMOUNT_OF_IMAGES = 18;
	static const int IMAGE_INDEX_COMPLETED = 3;
	static const int IMAGE_INDEX_NOT_COMPLETED = 4;
	static const int IMAGE_INDEX_ICON = 5;
	static const int AMOUNT_OF_ICON = 6;
	static const int IMAGE_INDEX_DOTS = 11;
	static const int AMOUNT_OF_HELP_IMAGE = 6;
	static const int IMAGE_INDEX_HELP = 12;

	MariaUI(MariaLogic *mariaLogic, QWidget *parent = 0);
	~MariaUI(void);
	
	void forceLogicCall();
	void setBackgroundColor(int r, int g, int b);
	static QPixmap* getImageHandler(int index);
	MariaUICommandBar* getCommandBar();

signals:	
	void triggerShowHideEvent();

public slots:
	void quitAction();

protected:
	bool eventFilter(QObject* obj, QEvent *event);
	void keyReleaseEvent(QKeyEvent* event);

protected slots:
	void updateBackgroundColor();
	void showHideEvent();

private:
	static const int BACKGROUND_UPDATE_FREQUENCY = 50;
	static const float FLOW_FACTOR;
	static const float VALUE_THRESHOLD;
	static const float CLOSE_BUTTON_X_OFFSET;
	static const float CLOSE_BUTTON_Y_OFFSET;

	MariaLogic *_mariaLogic;
	MariaUICommandBar* _commandBar;

	QToolButton *_btClose;
	QTimer *_bkgColorUpdateTimer;
	QColor _bkgColor;
	QColor _targetBkgColor;
	QSystemTrayIcon *_trayIcon;
	static QPixmap *_taskTypeIconHandler[AMOUNT_OF_IMAGES];

	void initWindow();
	void initButton();
	void initCommandBar();
	void initTrayIcon();
	void initBackgroundColor(int r, int g, int b);
	static void loadImages();
	static void unloadImages();
};