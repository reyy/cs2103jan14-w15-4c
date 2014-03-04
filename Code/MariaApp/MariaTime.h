#pragma once
#include <time.h>
#include <string>
using namespace std;

class MariaTime {
public:
	MariaTime(int year = 1901, int month = 1, int day = 1, int hour = 0, int min = 0, int sec = 0);
	MariaTime(string value, string format);

	void set(string value, string format);
	string get(string format);

	time_t getUnixTime();
	int getYear();
	int getMonth();
	int getDay();
	int getHour();
	int getMin();
	int getSec();

private:
	time_t timeStamp;
	struct tm timeStruct;
};

