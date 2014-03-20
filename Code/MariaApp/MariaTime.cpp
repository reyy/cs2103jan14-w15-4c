#include "MariaTime.h"

const string MariaTime::timeSeparator = ":";
const string MariaTime::timeMorning = " AM";
const string MariaTime::timeEvening = " PM";

const char * const MariaTime::DAYS[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
const char * const MariaTime::MONTHS[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November" , "December"};

MariaTime::MariaTime(int year , int month , int day , int hour , int min , int sec ) {
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
	timeStruct	 = *(localtime(&unixTime));
}

MariaTime::MariaTime(string value) {
	int year;
	int month;
	sscanf (value.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &timeStruct.tm_mday, &timeStruct.tm_hour, &timeStruct.tm_min, &timeStruct.tm_sec);
	timeStruct.tm_year = year - YEAR_OFFSET;
	timeStruct.tm_mon = month - MONTH_OFFSET;
	timeStamp = mktime(&timeStruct);
	
}


string MariaTime::get(string format) {
	char buff[20];
	strftime(buff, 20, format.c_str() , localtime(&timeStamp));
	return buff;
}

void MariaTime::set(time_t unixTime) {
	timeStamp = unixTime;
	time(&unixTime);
	timeStruct	 = *(localtime(&unixTime));
}

time_t MariaTime::get() {
	return timeStamp;
}

int MariaTime::getYear() {
	return timeStruct.tm_year + YEAR_OFFSET;
}

int MariaTime::getMonth() {
	return timeStruct.tm_mon + MONTH_OFFSET;
}

int MariaTime::getDay() {
	return timeStruct.tm_mday;
}
	
int MariaTime::getHour() {
	return timeStruct.tm_hour;
}
	
int MariaTime::getMin() {
	return timeStruct.tm_min;
}

int MariaTime::getSec() {
	return timeStruct.tm_sec;
}

int MariaTime::getDayWeek() {
	return timeStruct.tm_wday;
}

void MariaTime::setYear(int year) {
	timeStruct.tm_year = year - YEAR_OFFSET;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setMonth(int month) {
	timeStruct.tm_mon = month - MONTH_OFFSET;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setDay(int day) {
	timeStruct.tm_mday = day;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setHour(int hour) {
	timeStruct.tm_hour = hour;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setMin(int min) {
	timeStruct.tm_min = min;
	timeStamp = mktime(&timeStruct);
}

void MariaTime::setSec(int sec) {
	timeStruct.tm_sec = sec;
	timeStamp = mktime(&timeStruct);
}

string MariaTime::getTimeFromNow() {
	time_t currentTime;
	time ( &currentTime );
	double difference = difftime(timeStamp, currentTime);

	if(difference < 0) {
		return "";
	}else if(difference > 60*60*24*30) {
		return to_string((int)difference / ( 60*60*24*30)) + " Months";
	}else if(difference > 60*60*24*7) {
		return to_string((int)difference / ( 60*60*24*7)) + " Weeks";
	}else if(difference > 60*60*24) {
		return to_string((int)difference / ( 60*60*24)) + " Days";
	} else {
		string returnString;
		if(((int)difference % 60) >0 ) {
			returnString += to_string((int)difference % 60) + " Secs";
		}
		difference /= 60;
		if(((int)difference % 60) >0 ) {
			returnString = to_string((int)difference % 60) + " Mins " + returnString;
		}
		difference /= 60;
		if(((int)difference % 24) >0 ) {
			returnString = to_string((int)difference % 24) + " Hrs " + returnString;
		}
		return returnString;
	}
	
}

MariaTime MariaTime::getCurrentTime() {
	
	time_t rawtime;
	time ( &rawtime );
	MariaTime toReturn(rawtime);
 //toReturn.timeStruct = *localtime ( &rawtime );
	//toReturn.timeStruct.tm_year += YEAR_OFFSET;
	return toReturn;
}

string MariaTime::convertTimeToString(MariaTime *time) {
	string toReturn;

	if(time->getHour()<12) {
		toReturn=std::to_string(time->getHour())+timeSeparator;
		if(time->getMin()<10) {
			toReturn+="0";
		}
		toReturn+=std::to_string(time->getMin())+timeMorning;
	} else {
		toReturn=std::to_string(time->getHour()-12)+timeSeparator;
		if(time->getMin()<10) {
			toReturn+="0";
		}
		toReturn+=std::to_string(time->getMin())+timeEvening;
	}

	return toReturn;
}


int MariaTime::timeDifference(MariaTime *end, MariaTime *start) {
	return (int)difftime(end->get(), start->get());
}

int MariaTime::compareTo(MariaTime other) {
	double difference = difftime(get(), other.get());
	if( difference > 0 ){
		return 1;
	}else if( difference < 0 ){
		return -1;
	}
	return 0;
}
