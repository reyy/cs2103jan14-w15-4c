#include "MariaUIPreview.h"
const string MariaUIPreview::PREVIEW_EVENT_TOMORROW_NONE = "You have no events scheduled for tomorrow.\n";
const string MariaUIPreview::PREVIEW_EVENT_TOMORROW = "There is one event scheduled for %s tomorrow.\n";
const string MariaUIPreview::PREVIEW_EVENT_TOMORROW_MULTIPLE = "There are %i events scheduled and the first one starts at %s tomorrow.\n";

const string MariaUIPreview::PREVIEW_EVENT_TODAY_NONE = "There are no scheduled events today.\n";
const string MariaUIPreview::PREVIEW_EVENT_TODAY_FIRST_AT = "The first thing on your calendar today is '%s', at %s.\n";
const string MariaUIPreview::PREVIEW_EVENT_TODAY_FIRST_REMAINING_TIME = "The first thing on your calendar today is '%s', in %s minutes.\n";
const string MariaUIPreview::PREVIEW_EVENT_TODAY_NEXT_AT = "'%s' is next up on your calendar, at %s.\n";
const string MariaUIPreview::PREVIEW_EVENT_TODAY_NEXT_REMAINING_TIME = "'%s' is next up on your calendar, in %s minutes.\n";

const string MariaUIPreview::PREVIEW_DEADLINE_TOMORROW = "One task is due by %s tomorrow.\n";
const string MariaUIPreview::PREVIEW_DEADLINE_TOMORROW_MUTIPLE = "%i tasks are due tomorrow.\n";

const string MariaUIPreview::PREVIEW_DEADLINE_TODAY_AT = "One task is due today.\nIt is '%s' and is due by %s.\n";
const string MariaUIPreview::PREVIEW_DEADLINE_TODAY_REMAINING_TIME = "One task is due today It is '%s' and is due in %s minutes.\n";
const string MariaUIPreview::PREVIEW_DEADLINE_TODAY_MUTIPLE = "%i tasks are due today.\n";
const string MariaUIPreview::PREVIEW_DEADLINE_ITEM_AT = "'%s' is due by %s.\n";
const string MariaUIPreview::PREVIEW_DEADLINE_ITEM_REMAINING_TIME = "'%s' is due in %s minutes.\n";
const string MariaUIPreview::PREVIEW_DEADLINE_ITEM_TRIM = "Here are some of the tasks:\n";

const string MariaUIPreview::PREVIEW_FLOATING_SUGGESTION_DEFAULT = "'%s' was created since %s, was it done yet?\n";
const string MariaUIPreview::PREVIEW_FREE_DAY = "You are free on '%s'.\n";

const float MariaUIPreview::START_HEIGHT_SCALE = 0.25;
const float MariaUIPreview::MESSAGE_HEIGHT = 220;
const float MariaUIPreview::TODAYBAR_STARTHEIGHT_SCALE = 0.22;
const float MariaUIPreview::TOMORROWBAR_STARTHEIGHT_SCALE = 0.42;
const float MariaUIPreview::CALENDARBAR_STARTHEIGHT_SCALE = 0.53;
const float MariaUIPreview::TITLE_SEPARATE_HEIGHT = 12.0;
const float MariaUIPreview::TITLE_AREA_HEIGHT = 128.0;
const float MariaUIPreview::TITLE_XOFFSET = 30.0;
const float MariaUIPreview::BODY_XOFFSET = 10.0;
const float MariaUIPreview::DIVIDER_HEIGHT = 4.0;

MariaUIPreview::MariaUIPreview(QMainWindow *qmainWindow) {
	_qmainWindow=qmainWindow;

	_textToday = new QLabel(_qmainWindow);
	_textToday->setStyleSheet("color:#ffffff; font-size:10px;");
	_textToday->setText("Today");
	_textToday->setGeometry(QRect(-_qmainWindow->width(), -_qmainWindow->height(), 0.0, 0.0));
	_textToday->show();

	_textTodayBody = new QLabel(_qmainWindow);
	_textTodayBody->setStyleSheet("color:#ffffff; font-size:12px;");
	_textTodayBody->setAlignment(Qt::AlignTop);
	_textTodayBody->setWordWrap(true);
	_textTodayBody->setGeometry(QRect(-_qmainWindow->width(), -_qmainWindow->height(), 0.0, 0.0));
	_textTodayBody->show();

	_lineToday = new QLabel(_qmainWindow);
	_lineToday->setPixmap(QPixmap("./Resources/ui_divider.png"));
	_lineToday->setGeometry(QRect(-_qmainWindow->width(), -_qmainWindow->height(), 0.0, 0.0));
	_lineToday->show();

	_textTomorrow = new QLabel(_qmainWindow);
	_textTomorrow->setStyleSheet("color:#ffffff; font-size:10px;");
	_textTomorrow->setText("Tomorrow");
	_textTomorrow->setGeometry(QRect(-_qmainWindow->width(), -_qmainWindow->height(), 0.0, 0.0));
	_textTomorrow->show();

	_textTomorrowBody = new QLabel(_qmainWindow);
	_textTomorrowBody->setStyleSheet("color:#ffffff; font-size:12px;");
	_textTomorrowBody->setAlignment(Qt::AlignTop);
	_textTomorrowBody->setWordWrap(true);
	_textTomorrowBody->setGeometry(QRect(-_qmainWindow->width(), -_qmainWindow->height(), 0.0, 0.0));
	_textTomorrowBody->show();

	_lineTomorrow = new QLabel(_qmainWindow);
	_lineTomorrow->setPixmap(QPixmap("./Resources/ui_divider.png"));
	_lineTomorrow->setGeometry(QRect(-_qmainWindow->width(), -_qmainWindow->height(), 0.0, 0.0));
	_lineTomorrow->show();

	_textCalendar = new QLabel(_qmainWindow);
	_textCalendar->setStyleSheet("color:#ffffff; font-size:10px;");
	_textCalendar->setText("Calendar");
	_textCalendar->setGeometry(QRect(-_qmainWindow->width(), -_qmainWindow->height(), 0.0, 0.0));
	_textCalendar->show();

	_textCalendarBody = new QLabel(_qmainWindow);
	_textCalendarBody->setStyleSheet("color:#ffffff; font-size:12px;");
	_textCalendarBody->setAlignment(Qt::AlignTop);
	_textCalendarBody->setWordWrap(true);
	_textCalendarBody->setGeometry(QRect(-_qmainWindow->width(), -_qmainWindow->height(), 0.0, 0.0));
	_textCalendarBody->show();

	_lineCalendar = new QLabel(_qmainWindow);
	_lineCalendar->setPixmap(QPixmap("./Resources/ui_divider.png"));
	_lineCalendar->setGeometry(QRect(-_qmainWindow->width(), -_qmainWindow->height(), 0.0, 0.0));
	_lineCalendar->show();

	_generatedTodayText = "";
	_generatedTomorrowText = "";
	_generatedSuggestionText = "";
}

MariaUIPreview::~MariaUIPreview() {
	delete _lineCalendar;
	delete _textCalendarBody;
	delete _textCalendar;
	delete _lineTomorrow;
	delete _textTomorrowBody;
	delete _textTomorrow;
	delete _lineToday;
	delete _textTodayBody;
	delete _textToday;
}

void MariaUIPreview::updateText() {
	_textTodayBody->setText(QString::fromStdString(_generatedTodayText));
	_textTomorrowBody->setText(QString::fromStdString(_generatedTomorrowText));
	_textCalendarBody->setText(QString::fromStdString( _generatedSuggestionText));
}

void MariaUIPreview::updateGUI(QPointF statePosition) {
	_textToday->setGeometry(QRect(statePosition.x() + TITLE_XOFFSET, statePosition.y() + _qmainWindow->height()*TODAYBAR_STARTHEIGHT_SCALE, _qmainWindow->width() - TITLE_XOFFSET * 2, TITLE_SEPARATE_HEIGHT));
	_textTodayBody->setGeometry(QRect(statePosition.x() + TITLE_XOFFSET + BODY_XOFFSET, statePosition.y() + _qmainWindow->height()*TODAYBAR_STARTHEIGHT_SCALE + TITLE_SEPARATE_HEIGHT + DIVIDER_HEIGHT, _qmainWindow->width() - TITLE_XOFFSET * 2 - BODY_XOFFSET, TITLE_AREA_HEIGHT));
	_lineToday->setGeometry(QRect(statePosition.x() + TITLE_XOFFSET, statePosition.y() + _qmainWindow->height()*TODAYBAR_STARTHEIGHT_SCALE + TITLE_SEPARATE_HEIGHT, _qmainWindow->width(), DIVIDER_HEIGHT));
	_textTomorrow->setGeometry(QRect(statePosition.x() + TITLE_XOFFSET, statePosition.y() + _qmainWindow->height()*TOMORROWBAR_STARTHEIGHT_SCALE, _qmainWindow->width() - TITLE_XOFFSET * 2, TITLE_SEPARATE_HEIGHT));
	_textTomorrowBody->setGeometry(QRect(statePosition.x() + TITLE_XOFFSET + BODY_XOFFSET, statePosition.y() + _qmainWindow->height()*TOMORROWBAR_STARTHEIGHT_SCALE + TITLE_SEPARATE_HEIGHT + DIVIDER_HEIGHT, _qmainWindow->width() - TITLE_XOFFSET * 2 - BODY_XOFFSET, TITLE_AREA_HEIGHT));
	_lineTomorrow->setGeometry(QRect(statePosition.x() + TITLE_XOFFSET, statePosition.y() + _qmainWindow->height()*TOMORROWBAR_STARTHEIGHT_SCALE + TITLE_SEPARATE_HEIGHT, _qmainWindow->width(), DIVIDER_HEIGHT));
	_textCalendar->setGeometry(QRect(statePosition.x() + TITLE_XOFFSET, statePosition.y() + _qmainWindow->height()*CALENDARBAR_STARTHEIGHT_SCALE, _qmainWindow->width() - TITLE_XOFFSET * 2, TITLE_SEPARATE_HEIGHT));
	_textCalendarBody->setGeometry(QRect(statePosition.x() + TITLE_XOFFSET + BODY_XOFFSET, statePosition.y() + _qmainWindow->height()*CALENDARBAR_STARTHEIGHT_SCALE + TITLE_SEPARATE_HEIGHT + DIVIDER_HEIGHT, _qmainWindow->width() - TITLE_XOFFSET * 2 - BODY_XOFFSET, TITLE_AREA_HEIGHT));
	_lineCalendar->setGeometry(QRect(statePosition.x() + TITLE_XOFFSET, statePosition.y() + _qmainWindow->height()*CALENDARBAR_STARTHEIGHT_SCALE + TITLE_SEPARATE_HEIGHT, _qmainWindow->width(), DIVIDER_HEIGHT));

}

string MariaUIPreview::generateTodayText(vector<MariaTask*> taskListNow, vector<MariaTask*> taskListAll, vector<MariaTask*> taskListDeadLine) {
	string toReturn;
	char buffer[STRING_BUFFER_SIZE];

	//Today's Task.
	if(taskListNow.size() > 0) {
		int withinTheHour = MariaTime::timeDifference(taskListNow.at(0)->getStart(), &MariaTime::getCurrentTime());

		if(taskListAll.size() - taskListNow.size() == 0) {
			if(withinTheHour >= 0 ) {//Check if event has passed.
				if(withinTheHour < 60 * 60 && withinTheHour > 1) {
					sprintf_s(buffer, PREVIEW_EVENT_TODAY_FIRST_REMAINING_TIME.c_str(), taskListNow.at(0)->getTitle().c_str(), std::to_string(withinTheHour/60).c_str());
				} else {
					sprintf_s(buffer, PREVIEW_EVENT_TODAY_FIRST_AT.c_str(), taskListNow.at(0)->getTitle().c_str(), MariaTime::convertToTimeString(taskListNow.at(0)->getStart()).c_str());
				}
				toReturn+=buffer;
			}
		} else {
			if(withinTheHour >= 0 ) {//Check if event has passed.
				if(withinTheHour < 60 * 60 && withinTheHour > 1) {
					sprintf_s(buffer, PREVIEW_EVENT_TODAY_NEXT_REMAINING_TIME.c_str(), taskListNow.at(0)->getTitle().c_str(), std::to_string(withinTheHour/60).c_str());
				} else {
					sprintf_s(buffer, PREVIEW_EVENT_TODAY_NEXT_AT.c_str(), taskListNow.at(0)->getTitle().c_str(), MariaTime::convertToTimeString(taskListNow.at(0)->getStart()).c_str());
				}
				toReturn+=buffer;
			}
		}
	} else {
		sprintf_s(buffer, PREVIEW_EVENT_TODAY_NONE.c_str());
		toReturn+=buffer;
	}

	//Today's Deadline.
	if(taskListDeadLine.size() > 0) {
		if(taskListDeadLine.size() == 1) {
			int withinTheHour = MariaTime::timeDifference(taskListDeadLine.at(0)->getEnd(), &MariaTime::getCurrentTime());

			if(withinTheHour >= 0 ) {//Check if event has passed.
				if(withinTheHour < 60 * 60 && withinTheHour > 1) {
					sprintf_s(buffer, PREVIEW_DEADLINE_TODAY_REMAINING_TIME.c_str(), taskListDeadLine.at(0)->getTitle().c_str(), std::to_string(withinTheHour/60).c_str());
				} else {
					sprintf_s(buffer, PREVIEW_DEADLINE_TODAY_AT.c_str(), taskListDeadLine.at(0)->getTitle().c_str(), MariaTime::convertToTimeString(taskListDeadLine.at(0)->getEnd()).c_str());
				}
				toReturn+=buffer;
			}
		} else {
			sprintf_s(buffer, PREVIEW_DEADLINE_TODAY_MUTIPLE.c_str(), taskListDeadLine.size());
			toReturn+=buffer;

			int maxTask = taskListDeadLine.size();
			if(maxTask > MAX_TASK_SHOWN) {
				maxTask = MAX_TASK_SHOWN;
				toReturn += PREVIEW_DEADLINE_ITEM_TRIM;
			}
			for(int i = 0; i < maxTask; i++){
				int withinTheHour = MariaTime::timeDifference(taskListDeadLine.at(i)->getEnd(), &MariaTime::getCurrentTime());

				if(withinTheHour >= 0 ) {//Check if event has passed.
					if(withinTheHour < 60 * 60 && withinTheHour > 1) {
						sprintf_s(buffer, PREVIEW_DEADLINE_ITEM_REMAINING_TIME.c_str(), taskListDeadLine.at(i)->getTitle().c_str(), std::to_string(withinTheHour/60).c_str());
					} else {
						sprintf_s(buffer, PREVIEW_DEADLINE_ITEM_AT.c_str(), taskListDeadLine.at(i)->getTitle().c_str(), MariaTime::convertToTimeString(taskListDeadLine.at(i)->getEnd()).c_str());
					}
					toReturn+=buffer;
				}
			}
		}
	}

	_generatedTodayText = toReturn;
	return toReturn;
}

string MariaUIPreview::generateTomorrowText(vector<MariaTask*> taskListTomorrow, vector<MariaTask*> taskListTomorrowDeadLine) {
	string toReturn;
	char buffer[STRING_BUFFER_SIZE];

	//Tomorrow's Task
	if(taskListTomorrow.size()>0) {
		if(taskListTomorrow.size()==1) {
			sprintf_s(buffer, PREVIEW_EVENT_TOMORROW.c_str(), MariaTime::convertToTimeString(taskListTomorrow.at(0)->getStart()).c_str());
			toReturn+=buffer;
		} else {
			sprintf_s(buffer, PREVIEW_EVENT_TOMORROW_MULTIPLE.c_str(), taskListTomorrow.size(), MariaTime::convertToTimeString(taskListTomorrow.at(0)->getStart()).c_str());
			toReturn+=buffer;
		}
	} else {
		sprintf_s(buffer, PREVIEW_EVENT_TOMORROW_NONE.c_str());
		toReturn+=buffer;
	}

	//Tomorrow's Deadline
	if(taskListTomorrowDeadLine.size() > 0) {
		if(taskListTomorrowDeadLine.size() == 1) {
			sprintf_s(buffer, PREVIEW_DEADLINE_TOMORROW.c_str(), MariaTime::convertToTimeString(taskListTomorrowDeadLine.at(0)->getEnd()).c_str());
			toReturn+=buffer;
		} else {
			sprintf_s(buffer, PREVIEW_DEADLINE_TOMORROW_MUTIPLE.c_str(), taskListTomorrowDeadLine.size());
			toReturn+=buffer;
		}
	}

	_generatedTomorrowText = toReturn;
	return toReturn;
}

string MariaUIPreview::generateSuggestionText(int day, vector<MariaTask*> taskListSuggest) {
	string toReturn;
	char buffer[STRING_BUFFER_SIZE];

	if(day >= 0 && day <=6) {
		sprintf_s(buffer, PREVIEW_FREE_DAY.c_str(), MariaTime::DAYS[day]);
		toReturn+=buffer;
	}

	if(taskListSuggest.size() > 0) {
		MariaTask* generatedSuggestionTask = taskListSuggest.at(rand() % taskListSuggest.size());

		string dateCreated = MariaTime::convertToDateString(generatedSuggestionTask->getCreated());
		sprintf_s(buffer, PREVIEW_FLOATING_SUGGESTION_DEFAULT.c_str(), generatedSuggestionTask->getTitle().c_str(), dateCreated.c_str());
		toReturn+=buffer;
	}
	
	_generatedSuggestionText = toReturn;
	return toReturn;
}