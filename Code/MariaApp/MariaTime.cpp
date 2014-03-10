#include "MariaTime.h"


MariaTime::MariaTime(int year , int month , int day , int hour , int min , int sec ){
	timeStruct.tm_year = year-1900;
	timeStruct.tm_mon = month-1;
	timeStruct.tm_mday = day;
	timeStruct.tm_hour = hour;
	timeStruct.tm_min = min;
	timeStruct.tm_sec = sec;

	timeStamp = mktime(&timeStruct);
}

MariaTime::MariaTime(time_t unixTime) {
	timeStamp = unixTime;
	time(&unixTime);
	timeStruct	=*(localtime(&unixTime));
}

MariaTime::MariaTime(string value, string format){
	strftime(strdup(value.c_str()), value.length(), format.c_str(), &timeStruct);
	timeStamp = mktime(&timeStruct);
}

void MariaTime::set(string value, string format){
	strftime(strdup(value.c_str()), value.length(), format.c_str(), &timeStruct);
	timeStamp = mktime(&timeStruct);
}

string MariaTime::get(string format){
	return "";
}

void MariaTime::set(time_t unixTime) {
	timeStamp = unixTime;
	time(&unixTime);
	timeStruct	=*(localtime(&unixTime));
}

time_t MariaTime::get() {
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

MariaTime MariaTime::getCurrentTime() {
	MariaTime toReturn;
	time_t rawtime;
	time ( &rawtime );
    toReturn.timeStruct = *localtime ( &rawtime );
	toReturn.timeStruct.tm_year+=1900;
	return toReturn;
}
