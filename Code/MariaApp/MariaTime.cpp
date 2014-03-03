#include "MariaTime.h"


MariaTime::MariaTime(int year , int month , int day , int hour , int min , int sec ){
	timeStruct.tm_year = year;
	timeStruct.tm_mon = month;
	timeStruct.tm_mday = day;
	timeStruct.tm_hour = hour;
	timeStruct.tm_min = min;
	timeStruct.tm_sec = sec;

	time_t timeStamp = mktime(&timeStruct);
}


void MariaTime::set(string value, string format){
	strftime(strdup(value.c_str()), value.length(), format.c_str(), &timeStruct);
	timeStamp = mktime(&timeStruct);
}

string MariaTime::get(string format){
	return "";
}

time_t MariaTime::getUnixTime(){
	return timeStamp;
}

int MariaTime::getYear(){
	return timeStruct.tm_year;
}

int MariaTime::getMonth(){
	return timeStruct.tm_mon + 1; //todo: remove magic
}

int MariaTime::getDay(){
	return timeStruct.tm_mday;
}
	
int MariaTime::getHour(){
	return timeStruct.tm_hour;
}
	
int MariaTime::getMin(){
	return timeStruct.tm_min;
}

int MariaTime::getSec(){
	return timeStruct.tm_sec;
}
