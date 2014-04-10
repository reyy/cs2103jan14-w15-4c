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
const string MariaUIPreview::PREVIEW_DEADLINE_ITEM_TRIM = "Here are some of the tasks.\n";

const string MariaUIPreview::PREVIEW_FLOATING_SUGGESTION_DEFAULT = "'%s' was created since %s, was it done yet?\n";

const float MariaUIPreview::START_HEIGHT_SCALE = 0.28;
const float MariaUIPreview::MESSAGE_HEIGHT = 220;

MariaUIPreview::MariaUIPreview(QMainWindow *qmainWindow) {
	_qmainWindow=qmainWindow;

	_mainText = new QLabel(_qmainWindow);
	_mainText->setAlignment(Qt::AlignJustify);
	_mainText->setStyleSheet("padding-left:" + QString::number(TEXTBOX_X_OFFSET) + "px;padding-right:" + QString::number(TEXTBOX_X_OFFSET) + "px; color:#ffffff; font-size:" + QString::number(FONT_SIZE) + "px;");
	_mainText->setWordWrap(true);
	_mainText->show();

	_generatedTodayText = "";
	_generatedTomorrowText = "";
	_generatedSuggestionText = "";
}

MariaUIPreview::~MariaUIPreview() {
	delete _mainText;
}

void MariaUIPreview::updateText() {
	string toPrint;
	string temp;

	temp += _generatedTodayText;

	if(temp.length()>0) {
		temp += "\n";
	}

	toPrint += temp;
	temp = _generatedTomorrowText;

	if(temp.length()>0) {
		temp += "\n";
	}

	toPrint += temp;
	toPrint += _generatedSuggestionText;

	_mainText->setText(QString::fromStdString(toPrint));
}

void MariaUIPreview::updateGUI(QPointF statePosition) {
	_mainText->setGeometry(QRect(statePosition.x(), 
		statePosition.y() + _qmainWindow->height()*START_HEIGHT_SCALE, _qmainWindow->width(), MESSAGE_HEIGHT));
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
		if(toReturn.length()>0) {
			toReturn+="\n";
		}

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
		if(toReturn.length()>0) {
			toReturn+="\n";
		}

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

string MariaUIPreview::generateSuggestionText(vector<MariaTask*> taskListSuggest) {
	string toReturn;
	char buffer[STRING_BUFFER_SIZE];

	if(taskListSuggest.size() > 0) {
		MariaTask* generatedSuggestionTask = taskListSuggest.at(rand() % taskListSuggest.size());

		string dateCreated = MariaTime::convertToDateString(generatedSuggestionTask->getCreated());
		sprintf_s(buffer, PREVIEW_FLOATING_SUGGESTION_DEFAULT.c_str(), generatedSuggestionTask->getTitle().c_str(), dateCreated.c_str());
		toReturn+=buffer;
	}
	
	_generatedSuggestionText = toReturn;
	return toReturn;
}