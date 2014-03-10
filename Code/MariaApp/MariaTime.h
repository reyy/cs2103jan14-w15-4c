#pragma once
#include <time.h>
#include <string>
using namespace std;

class MariaTime {
public:
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


	static MariaTime getCurrentTime();

private:
	time_t timeStamp;
	struct tm timeStruct;
};

