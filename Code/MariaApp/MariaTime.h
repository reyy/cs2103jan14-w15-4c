#pragma once
#include <time.h>
#include <string>
using namespace std;

class MariaTime {
public:
	static const string timeSeparator;
	static const string timeMorning;
	static const string timeEvening;

	static const int YEAR_OFFSET = 1900;
	static const int MONTH_OFFSET = 1;
	
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
	static string convertTimeToString(MariaTime *time);
	static int timeDifference(MariaTime *end, MariaTime *start);

private:
	time_t timeStamp;
	struct tm timeStruct;
};

