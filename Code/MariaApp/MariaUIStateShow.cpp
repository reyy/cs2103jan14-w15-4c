//@author A0111784H
#include <assert.h> 
#include "MariaMacros.h"
#include "MariaUIStateShow.h"
#include "MariaUI.h"

//@author generated
const float MariaUIStateShow::TASKBAR_STARTHEIGHT_SCALE = 0.1;
const float MariaUIStateShow::TASK_STARTHEIGHT_SCALE = 0.23;
const float MariaUIStateShow::TITLE_WIDTH = 420.0;
const float MariaUIStateShow::TITLE_HEIGHT = 50.0;
const float MariaUIStateShow::TITLE_HEIGHT_SCALE = 0.15;

MariaUIStateShow::MariaUIStateShow(QMainWindow* qmainWindow, string title, vector<MariaTask*> listOfTasks) : MariaUIStateDisplay(qmainWindow, TASK_STARTHEIGHT_SCALE, MAX_ITEM_IN_PAGE) {
	assert(qmainWindow != NULL);
	
	_listOfTasks = listOfTasks;

	_titleLabel = new QLabel(_qmainWindow);
	_titleLabel->setStyleSheet("color:#ffffff; font-size:22px; font-weight:bold;");
	_titleLabel->setAlignment(Qt::AlignCenter);
	_titleLabel->setText(QString::fromStdString(title));
	_titleLabel->hide();
}

MariaUIStateShow::~MariaUIStateShow() {
	clearUITask();
	SAFE_DELETE(_titleLabel);
}

//@author A0111784H
void MariaUIStateShow::updateGUI() {
}

void MariaUIStateShow::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height() * TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(BACKGROUND_R, BACKGROUND_G, BACKGROUND_B);

	_titleLabel->show();
}

void MariaUIStateShow::initActiveState() {
	for (MariaTask* temp : _listOfTasks) {
		addUITask(temp, MariaUITask::DisplayType::DETAILED);
	}

	updateUITaskPosition();
	updatePage();
}

void MariaUIStateShow::initEndState() {
	eraseAllUITask();
}

bool MariaUIStateShow::timerBeginState() {
	_titleLabel->setGeometry(QRect(getPosition().x() + _qmainWindow->width() * 0.5 - TITLE_WIDTH * 0.5, getPosition().y() + _qmainWindow->height() * TITLE_HEIGHT_SCALE, TITLE_WIDTH, TITLE_HEIGHT));
	updatePageTitleGUI();
	return false;
}

bool MariaUIStateShow::timerActiveState() {
	return false;
}

bool MariaUIStateShow::timerEndState() {
	_titleLabel->setGeometry(QRect(getPosition().x() + _qmainWindow->width() * 0.5 - TITLE_WIDTH * 0.5, getPosition().y() + _qmainWindow->height() * TITLE_HEIGHT_SCALE, TITLE_WIDTH, TITLE_HEIGHT));
	updatePageTitleGUI();
	return false;
}