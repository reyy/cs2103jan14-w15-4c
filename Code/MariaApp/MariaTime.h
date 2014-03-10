#pragma once
#include <time.h>
#include <string>
using namespace std;

class MariaTime {
public:
	static const int YEAR_OFFSET = 1900;
	static const int MONTH_OFFSET = 1;

	MariaTime(int year = 1901, int month = 1, int day = 1, int hour = 0, int min = 0, int sec = 0);
	MariaTime(time_t unixTime);
	MariaTime(string value);

	string get(string format);

	void set(time_t unixTime);
	time_t get();

	int getYear();
	int getMonth();
	int getDay();
	int getHour();
	int getMin();
	int getSec();

	void setYear(int);
	void setMonth(int);
	void setDay(int);
	void setHour(int);
	void setMin(int);
	void setSec(int);


	static MariaTime getCurrentTime();
	static MariaTime compareTime(MariaTime *a, MariaTime *b);

private:
	time_t timeStamp;
	struct tm timeStruct;
};

