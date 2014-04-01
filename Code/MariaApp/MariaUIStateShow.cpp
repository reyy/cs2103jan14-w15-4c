#include <assert.h> 
#include "MariaUIStateShow.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

const float MariaUIStateShow::TASKBAR_STARTHEIGHT_SCALE = 0.1;
const float MariaUIStateShow::TASK_STARTHEIGHT_SCALE = 0.2;
const float MariaUIStateShow::TITLE_WIDTH = 120.0;
const float MariaUIStateShow::TITLE_HEIGHT = 98.0;
const float MariaUIStateShow::TITLE_HEIGHT_SCALE = 0.1;

MariaUIStateShow::MariaUIStateShow(QMainWindow* qmainWindow, MariaTaskManager *taskManager, string title, vector<MariaTask*> listOfTasks) : MariaUIStateDisplay(qmainWindow, taskManager, TASK_STARTHEIGHT_SCALE) {
	_listOfTasks = listOfTasks;

	_titleLabel = new QLabel(_qmainWindow);
	_titleLabel->setStyleSheet("color:#ffffff; font-size:22px; font-weight:bold;");
	_titleLabel->setAlignment(Qt::AlignCenter);
	_titleLabel->setText(QString::fromStdString(title));
	_titleLabel->hide();
}

MariaUIStateShow::~MariaUIStateShow() {
	clearUITask();
	delete _titleLabel;
}

void MariaUIStateShow::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(116, 30, 168);

	_titleLabel->show();
}

void MariaUIStateShow::initActiveState() {
	for(MariaTask* temp : _listOfTasks) {
		addUITask(temp, MariaUITask::DISPLAY_TYPE::CONTRACTED);
	}
}

void MariaUIStateShow::initEndState() {
	eraseAllUITask();
}

bool MariaUIStateShow::timerBeginState() {
	_titleLabel->setGeometry(QRect(getPosition().x() + _qmainWindow->width()*0.5-60, getPosition().y() + _qmainWindow->height()*TITLE_HEIGHT_SCALE, TITLE_WIDTH, TITLE_HEIGHT));
	return false;
}

bool MariaUIStateShow::timerActiveState() {
	return false;
}

bool MariaUIStateShow::timerEndState() {
	_titleLabel->setGeometry(QRect(getPosition().x() + _qmainWindow->width()*0.5-60, getPosition().y() + _qmainWindow->height()*TITLE_HEIGHT_SCALE, TITLE_WIDTH, TITLE_HEIGHT));
	return false;
}

void MariaUIStateShow::updateGUI() {
}