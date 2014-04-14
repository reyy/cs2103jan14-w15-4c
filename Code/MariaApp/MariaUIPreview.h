//@author A0111784H
#pragma once

#include <string>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaTaskManager.h"

using namespace std;

class MariaUIPreview : QWidget {
public:
	
	static const int TEXTBOX_X_OFFSET = 30;
	static const int MAX_CHAR_LENGTH = 40;
	static const int FONT_SIZE = 12;
	static const int STRING_BUFFER_SIZE = 255;
	static const int MAX_TASK_SHOWN = 3;
	static const int DAY_OFFSET = 1;
	static const int DAY_OF_WEEK = 7;
	static const int MINUTES_IN_HOURS = 60;

	static const string PREVIEW_EVENT_TODAY_NONE;
	static const string PREVIEW_EVENT_TODAY_FIRST_AT;
	static const string PREVIEW_EVENT_TODAY_FIRST_REMAINING_TIME;
	static const string PREVIEW_EVENT_TODAY_NEXT_AT;
	static const string PREVIEW_EVENT_TODAY_NEXT_REMAINING_TIME;

	static const string PREVIEW_EVENT_TOMORROW_NONE;
	static const string PREVIEW_EVENT_TOMORROW;
	static const string PREVIEW_EVENT_TOMORROW_MULTIPLE;
	static const string PREVIEW_DEADLINE_TOMORROW;
	static const string PREVIEW_DEADLINE_TOMORROW_MUTIPLE;

	static const string PREVIEW_DEADLINE_TODAY_AT;
	static const string PREVIEW_DEADLINE_TODAY_REMAINING_TIME;
	static const string PREVIEW_DEADLINE_TODAY_MUTIPLE;
	static const string PREVIEW_DEADLINE_ITEM_AT;
	static const string PREVIEW_DEADLINE_ITEM_REMAINING_TIME;
	static const string PREVIEW_DEADLINE_ITEM_TRIM;
	
	static const string PREVIEW_FLOATING_SUGGESTION_DEFAULT;
	static const string PREVIEW_FREE_DAY;
	static const string PREVIEW_FREE_DAY_TODAY;
	
	static const float START_HEIGHT_SCALE;
	static const float CALENDAR_HEIGHT_SCALE;
	static const float MESSAGE_HEIGHT;
	static const float SPACING_HEIGHT;
	static const float TITLE_SEPARATE_HEIGHT;
	static const float TITLE_AREA_HEIGHT;
	static const float BODY_XOFFSET;
	static const float DIVIDER_HEIGHT;

	MariaUIPreview(QMainWindow *qmainWindow);
	~MariaUIPreview();

	string generateTodayText(vector<MariaTask*> taskListNow, vector<MariaTask*> taskListAll, vector<MariaTask*> taskListDeadLine);
	string generateTomorrowText(vector<MariaTask*> taskListTomorrow, vector<MariaTask*> taskListTomorrowDeadLine);
	string generateSuggestionText(int day, vector<MariaTask*> taskListSuggest);
	
	void updateGUI(QPointF statePosition);
	void updateText();

private:
	QMainWindow *_qmainWindow;

	QLabel *_textToday;
	QLabel *_textTodayBody;
	QLabel *_lineToday;
	QLabel *_textTomorrow;
	QLabel *_textTomorrowBody;
	QLabel *_lineTomorrow;
	QLabel *_textCalendar;
	QLabel *_textCalendarBody;
	QLabel *_lineCalendar;

	string _generatedTodayText;
	string _generatedTomorrowText;
	string _generatedSuggestionText;

	int endLineCount(string text);
};

