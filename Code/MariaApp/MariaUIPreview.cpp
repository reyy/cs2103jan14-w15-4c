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

MariaUIPreview::MariaUIPreview(QMainWindow *qmainWindow,MariaTaskManager *taskManager) {
	_qmainWindow=qmainWindow;
	_taskManager=taskManager;

	_mainText = new QLabel(_qmainWindow);
	_mainText->setAlignment(Qt::AlignJustify);
	_mainText->setStyleSheet("padding-left:" + QString::number(TEXTBOX_X_OFFSET) + "px;padding-right:" + QString::number(TEXTBOX_X_OFFSET) + "px; color:#ffffff; font-size:" + QString::number(FONT_SIZE) + "px;");
	_mainText->setWordWrap(true);
	_mainText->hide();

	_generatedSuggestionTask = NULL;

	_updateTextTimer = new QTimer(this);
	connect(_updateTextTimer, SIGNAL(timeout()), this, SLOT(updateText()));
}

MariaUIPreview::~MariaUIPreview() {
	delete _updateTextTimer;
	delete _mainText;
}

string MariaUIPreview::generateTodayText() {
	string toReturn;

	MariaTime now=MariaTime::getCurrentTime();
	MariaTime startOfDay(now.getYear(),now.getMonth(), now.getDay(), 0, 0);
	MariaTime endOfDay(now.getYear(),now.getMonth(), now.getDay(), 23, 59);

	char buffer[STRING_BUFFER_SIZE];

	//Today's Task.
	vector<MariaTask*> taskListNow = _taskManager->findTask(&now,&endOfDay, MariaTask::TaskType::TIMED, false);
	vector<MariaTask*> taskListAll = _taskManager->findTask(&startOfDay,&endOfDay, MariaTask::TaskType::TIMED, false);
	
	if(taskListNow.size() > 0) {
		int withinTheHour = MariaTime::timeDifference(taskListNow.at(0)->getStart(), &now);

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
		/* MAY BE REMOVED.
		sprintf_s(buffer, PREVIEW_EVENT_TODAY_NONE.c_str());
		toReturn+=buffer;
		*/
	}

	//Today's Deadline.
	vector<MariaTask*> taskListDeadLine = _taskManager->findTask(&now,&endOfDay, MariaTask::TaskType::DEADLINE, false);

	if(taskListDeadLine.size() > 0) {
		if(toReturn.length()>0) {
			toReturn+="\n\n";
		}

		if(taskListDeadLine.size() == 1) {
			int withinTheHour = MariaTime::timeDifference(taskListDeadLine.at(0)->getEnd(), &now);

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
				int withinTheHour = MariaTime::timeDifference(taskListDeadLine.at(0)->getEnd(), &now);
				
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

	return toReturn;
}

string MariaUIPreview::generateTomorrowText() {
	string toReturn;

	MariaTime now=MariaTime::getCurrentTime();
	MariaTime startOfTomorrow(now.getYear(),now.getMonth(), now.getDay()+1, 0, 0);
	MariaTime endOfTomorrow(now.getYear(),now.getMonth(), now.getDay()+1, 23, 59);

	char buffer[STRING_BUFFER_SIZE];
	
	//Tomorrow's Task
	vector<MariaTask*> taskList = _taskManager->findTask(&startOfTomorrow,&endOfTomorrow, MariaTask::TaskType::TIMED, false);

	if(taskList.size()>0) {
		if(taskList.size()==1) {
			sprintf_s(buffer, PREVIEW_EVENT_TOMORROW.c_str(), MariaTime::convertToTimeString(taskList.at(0)->getStart()).c_str());
			toReturn+=buffer;
		} else {
			sprintf_s(buffer, PREVIEW_EVENT_TOMORROW_MULTIPLE.c_str(), taskList.size(), MariaTime::convertToTimeString(taskList.at(0)->getStart()).c_str());
			toReturn+=buffer;
		}
	} else {
		/* MAY BE REMOVED
		sprintf_s(buffer, PREVIEW_EVENT_TOMORROW_NONE.c_str());
		toReturn+=buffer;
		*/
	}

	//Tomorrow's Deadline
	vector<MariaTask*> taskListDeadLine = _taskManager->findTask(&startOfTomorrow,&endOfTomorrow, MariaTask::TaskType::DEADLINE, false);

	if(taskListDeadLine.size() > 0) {
		if(toReturn.length()>0) {
			toReturn+="\n\n";
		}

		if(taskListDeadLine.size() == 1) {
			sprintf_s(buffer, PREVIEW_DEADLINE_TOMORROW.c_str(), MariaTime::convertToTimeString(taskListDeadLine.at(0)->getEnd()).c_str());
			toReturn+=buffer;
		} else {
			sprintf_s(buffer, PREVIEW_DEADLINE_TOMORROW_MUTIPLE.c_str(), taskListDeadLine.size());
			toReturn+=buffer;
		}
	}

	return toReturn;
}

string MariaUIPreview::generateSuggestionText(bool force) {
	string toReturn;

	vector<MariaTask*> taskList = _taskManager->findTask(MariaTask::TaskType::FLOATING, false);

	char buffer[STRING_BUFFER_SIZE];

	if(taskList.size() > 0) {
		if(_generatedSuggestionTask == NULL||force) {
			_generatedSuggestionTask = taskList.at(rand() % taskList.size());
		} else {
			string dateCreated = MariaTime::convertToDateString(_generatedSuggestionTask->getCreated());
			sprintf_s(buffer, PREVIEW_FLOATING_SUGGESTION_DEFAULT.c_str(), _generatedSuggestionTask->getTitle().c_str(), dateCreated.c_str());
			toReturn+=buffer;
		}
	} else {
		_generatedSuggestionTask = NULL;
	}

	return toReturn;
}

void MariaUIPreview::updateText() {
	string toPrint;
	string temp;

	temp += generateTodayText();

	if(temp.length()>0) {
		temp+="\n";
	}

	toPrint+=temp;
	temp = generateTomorrowText();

	if(temp.length()>0) {
		temp+="\n";
	}

	toPrint+=temp;
	toPrint+=generateSuggestionText(false);

	_mainText->setText(QString::fromStdString(toPrint));
}

void MariaUIPreview::startUpdating() {
	if(!_updateTextTimer->isActive()) {
		_updateTextTimer->start(UPDATE_FREQUENCY);
		_mainText->show();
		updateText();
	}
}

void MariaUIPreview::updateGUI(QPointF statePosition) {
	_mainText->setGeometry(QRect(statePosition.x(), 
		statePosition.y() + _qmainWindow->height()*START_HEIGHT_SCALE, _qmainWindow->width(), MESSAGE_HEIGHT));
}