#include "MariaTime.h"


MariaTime::MariaTime(int year , int month , int day , int hour , int min , int sec ){
	timeStruct.tm_year = year - YEAR_OFFSET;
	timeStruct.tm_mon = month - MONTH_OFFSET;
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

MariaTime::MariaTime(string value){
	int year;
	sscanf (value.c_str(),"%d-%d-%d %d:%d:%d",&year,&timeStruct.tm_mon,&timeStruct.tm_mday,&timeStruct.tm_hour,&timeStruct.tm_min,&timeStruct.tm_sec);
	timeStruct.tm_year = year - YEAR_OFFSET;

	timeStamp = mktime(&timeStruct);
	
}


string MariaTime::get(string format){
	char buff[20];
	strftime(buff, 20, format.c_str() , localtime(&timeStamp));
	return buff;
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
	return timeStruct.tm_mon + MONTH_OFFSET;
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

void MariaTime::setYear(int year){
	timeStruct.tm_year = year - YEAR_OFFSET;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setMonth(int month){
	timeStruct.tm_mon = month - MONTH_OFFSET;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setDay(int day){
	timeStruct.tm_mday = day;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setHour(int hour){
	timeStruct.tm_hour = hour;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setMin(int min){
	timeStruct.tm_min = min;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setSec(int sec){
	timeStruct.tm_sec = sec;
	timeStamp = mktime(&timeStruct);
}

MariaTime MariaTime::getCurrentTime() {
	MariaTime toReturn;
	time_t rawtime;
	time ( &rawtime );
    toReturn.timeStruct = *localtime ( &rawtime );
	toReturn.timeStruct.tm_year += YEAR_OFFSET;
	return toReturn;
}

MariaTime MariaTime::compareTime(MariaTime *a, MariaTime *b){
	double difference = difftime(a->get(), b->get());
	MariaTime toReturn;
	toReturn.setSec((int)difference % 60);
	difference /= 60;
	toReturn.setMin((int)difference%(60));
	difference /= 60;
	toReturn.setHour((int)difference%(24));
	difference /= 24;
	toReturn.setDay((int)difference);
	return toReturn;
}