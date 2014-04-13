#pragma once
#include <time.h>
#include <string>
using namespace std;

class MariaTime {
public:
	static const string TIME_AM;
	static const string TIME_PM;	
	static const string TIME_SEPARATOR;
	static const string DATE_SEPARATOR;
	static const string DATE_TIME_FORMAT;

	static const string STRING_MONTH;
	static const string STRING_WEEK;
	static const string STRING_DAY;
	static const string STRING_HOUR;
	static const string STRING_MIN;
	static const string STRING_SEC;
	static const string STRING_PLURAL;
	static const string STRING_EMPTY;
	static const string STRING_ZERO_PAD;

	static const int YEAR_OFFSET = 1900;
	static const int MONTH_OFFSET = 1;
	static const int AM_PM_OFFSET = 12;
	static const int DOUBLE_DIGIT_LIMIT = 10;
	static const int BUFFER_SIZE = 20;

	static const int NUM_SEC = 60;
	static const int NUM_MIN = 60;
	static const int NUM_HRS = 24;
	static const int NUM_DAYS = 7;
	static const int NUM_DAYS_PER_MONTH = 30;
	static const int NUM_SECS_PER_MONTH = NUM_SEC * NUM_MIN * NUM_HRS * NUM_DAYS_PER_MONTH;
	static const int NUM_SECS_PER_WEEK = NUM_SEC * NUM_MIN * NUM_HRS * NUM_DAYS;
	static const int NUM_SECS_PER_DAY = NUM_SEC * NUM_MIN * NUM_HRS;
	
	static const char * const DAYS[];
	static const char * const MONTHS[];

	MariaTime(int year = 1901, int month = 1, int day = 1, int hour = 0, int min = 0, int sec = 0);
	MariaTime(time_t unixTime);
	MariaTime(string value);
	
	string getTimeFromNow();
	string get(string format);
	time_t get();

	void set(time_t unixTime);

	int getYear();
	int getMonth();
	int getDay();
	int getHour();
	int getMin();
	int getSec();
	int getDayWeek();

	void setYear(int);
	void setMonth(int);
	void setDay(int);
	void setHour(int);
	void setMin(int);
	void setSec(int);

	int compareTo(MariaTime other);

	static MariaTime getCurrentTime();
	static string convertToTimeString(MariaTime *time);
	static string convertToDateString(MariaTime *time);
	static string convertToMonthString(MariaTime *time);
	static int timeDifference(MariaTime *end, MariaTime *start);

private:
	time_t timeStamp;
	struct tm timeStruct;

	string formatPlural(int value, string text);
};

