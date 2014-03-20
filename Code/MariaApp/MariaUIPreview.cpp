#include "MariaUIPreview.h"
const string MariaUIPreview::PREVIEW_EVENT_TOMORROW_NONE = "You have no events scheduled for tomorrow.";
const string MariaUIPreview::PREVIEW_EVENT_TOMORROW = "There is one event scheduled for %s.";
const string MariaUIPreview::PREVIEW_EVENT_TOMORROW_MULTIPLE = "There are %i events scheduled and the first one starts at %s.";

const string MariaUIPreview::PREVIEW_EVENT_TODAY_FIRST_AT = "The first thing on your calendar today is '%s', at %s.";
const string MariaUIPreview::PREVIEW_EVENT_TODAY_FIRST_REMAINING_TIME = "The first thing on your calendar today is '%s', in %s minutes.";
const string MariaUIPreview::PREVIEW_EVENT_TODAY_NEXT_AT = "'%s' is next up on your calendar, at %s.";
const string MariaUIPreview::PREVIEW_EVENT_TODAY_NEXT_REMAINING_TIME = "'%s' is next up on your calendar, in %s minutes.";

const string MariaUIPreview::PREVIEW_DEADLINE_TOMORROW = "One task is due tomorrow. Is is '%s' by %s.";
const string MariaUIPreview::PREVIEW_DEADLINE_TOMORROW_MUTIPLE = "%s tasks are due tomorrow.\n";

const string MariaUIPreview::PREVIEW_DEADLINE_TODAY = "One task is due today Is is '%s' by %s";
const string MariaUIPreview::PREVIEW_DEADLINE_TODAY_MUTIPLE = "%s tasks are due today.\n";
const string MariaUIPreview::PREVIEW_DEADLINE_ITEM = "%s is due by %s.\n";

const string MariaUIPreview::PREVIEW_FLOATING_SUGGESTION_NO_EVENT = "You have no events today. Maybe you try clearing '%s'? It was created since $s.";
const string MariaUIPreview::PREVIEW_FLOATING_SUGGESTION_DEFAULT = "'%s' was created since %s, was it done yet?";

const float MariaUIPreview::START_HEIGHT_SCALE = 0.2;
const float MariaUIPreview::MESSAGE_HEIGHT = 320;

MariaUIPreview::MariaUIPreview(QMainWindow *qmainWindow,MariaTaskManager *taskManager) {
	_qmainWindow=qmainWindow;
	_taskManager=taskManager;

	_mainText = new QLabel(_qmainWindow);
	_mainText->setAlignment(Qt::AlignJustify);
	_mainText->setStyleSheet("color:#ffffff; font-size:"+QString::number(FONT_SIZE)+"px;");
	_mainText->setWordWrap(true);
	_mainText->hide();

	_updateTextTimer = new QTimer(this);
	connect(_updateTextTimer, SIGNAL(timeout()), this, SLOT(updateText()));
}

MariaUIPreview::~MariaUIPreview() {
	delete _updateTextTimer;
	delete _mainText;
}

string MariaUIPreview::updateTodayText() {
	string toReturn;
	
	MariaTime now=MariaTime::getCurrentTime();
	MariaTime startOfDay(now.getYear(),now.getMonth(), now.getDay(), 0, 0);
	MariaTime endOfDay(now.getYear(),now.getMonth(), now.getDay(), 23, 59);
	
	char buffer[STRING_BUFFER_SIZE];

	vector<MariaTask*> taskListNow = _taskManager->findTask(&now,&endOfDay, MariaTask::TaskType::TIMED);
	vector<MariaTask*> taskListAll = _taskManager->findTask(&startOfDay,&endOfDay, MariaTask::TaskType::TIMED);

	//Today's Task.
	if(taskListNow.size() > 0) {
		int withinTheHour = MariaTime::timeDifference(taskListNow.at(0)->getStart(), &now);

		if(taskListAll.size() - taskListNow.size() == 0) {
			if(withinTheHour >= 0 ) {//Check if event has passed.
				if(withinTheHour < 60 * 60 && withinTheHour > 1) {
					sprintf_s(buffer, PREVIEW_EVENT_TODAY_FIRST_REMAINING_TIME.c_str(), taskListNow.at(0)->getTitle().c_str(), std::to_string(withinTheHour/60).c_str());
				} else {
					sprintf_s(buffer, PREVIEW_EVENT_TODAY_FIRST_AT.c_str(), taskListNow.at(0)->getTitle().c_str(), MariaTime::convertTimeToString(taskListNow.at(0)->getStart()).c_str());
				}
				toReturn+=buffer;
			}
		} else {
			if(withinTheHour >= 0 ) {//Check if event has passed.
				if(withinTheHour < 60 * 60 && withinTheHour > 1) {
					sprintf_s(buffer, PREVIEW_EVENT_TODAY_NEXT_REMAINING_TIME.c_str(), taskListNow.at(0)->getTitle().c_str(), std::to_string(withinTheHour/60).c_str());
				} else {
					sprintf_s(buffer, PREVIEW_EVENT_TODAY_NEXT_AT.c_str(), taskListNow.at(0)->getTitle().c_str(), MariaTime::convertTimeToString(taskListNow.at(0)->getStart()).c_str());
				}
				toReturn+=buffer;
			}
		}
	}

	return toReturn;
}

string MariaUIPreview::updateTomorrowText() {
	string toReturn;

	MariaTime now=MariaTime::getCurrentTime();
	MariaTime startOfTomorrow(now.getYear(),now.getMonth(), now.getDay()+1, 0, 0);
	MariaTime endOfTomorrow(now.getYear(),now.getMonth(), now.getDay()+1, 23, 59);

	char buffer[STRING_BUFFER_SIZE];

	vector<MariaTask*> taskList = _taskManager->findTask(&startOfTomorrow,&endOfTomorrow, MariaTask::TaskType::TIMED);
	
	if(taskList.size()>0) {
		if(taskList.size()==1) {
			sprintf_s(buffer, PREVIEW_EVENT_TOMORROW.c_str(), MariaTime::convertTimeToString(taskList.at(0)->getStart()).c_str());
			toReturn+=buffer;
		} else {
			sprintf_s(buffer, PREVIEW_EVENT_TOMORROW_MULTIPLE.c_str(), taskList.size(), MariaTime::convertTimeToString(taskList.at(0)->getStart()).c_str());
			toReturn+=buffer;
		}
	} else {
		sprintf_s(buffer, PREVIEW_EVENT_TOMORROW_NONE.c_str());
		toReturn+=buffer;
	}

	return toReturn;
}

string MariaUIPreview::updateSuggestionText() {
	return "";
}

void MariaUIPreview::updateText() {
	

	_mainText->setText(QString::fromStdString(updateTodayText() + "\n\n" + updateTomorrowText() + "\n\n" + updateSuggestionText()));
}

void MariaUIPreview::startUpdating() {
	if(!_updateTextTimer->isActive()) {
		_updateTextTimer->start(UPDATE_FREQUENCY);
		_mainText->show();
		updateText();
	}
}

void MariaUIPreview::updateGUI(QPointF statePosition) {
	_mainText->setGeometry(QRect(statePosition.x() + TEXTBOX_X_OFFSET, 
		statePosition.y() + _qmainWindow->height()*START_HEIGHT_SCALE, _qmainWindow->width()-2*TEXTBOX_X_OFFSET, MESSAGE_HEIGHT));
}