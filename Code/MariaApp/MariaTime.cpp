//@author A0111823U
#include "MariaTime.h"

const string MariaTime::TIME_SEPARATOR = ":";
const string MariaTime::TIME_AM = " AM";
const string MariaTime::TIME_PM = " PM";
const string MariaTime::DATE_SEPARATOR = " ";
const string MariaTime::DATE_TIME_FORMAT = "%d-%d-%d %d:%d:%d";

const string MariaTime::STRING_MONTH = " month";
const string MariaTime::STRING_WEEK = " week";
const string MariaTime::STRING_DAY = " day";
const string MariaTime::STRING_HOUR = " hr ";
const string MariaTime::STRING_MIN = " min ";
const string MariaTime::STRING_SEC = " sec";
const string MariaTime::STRING_PLURAL = " s";
const string MariaTime::STRING_EMPTY = "";
const string MariaTime::STRING_ZERO_PAD = "0";

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
	sscanf (value.c_str(), DATE_TIME_FORMAT.c_str(), &year, &month, &timeStruct.tm_mday, &timeStruct.tm_hour, &timeStruct.tm_min, &timeStruct.tm_sec);
	timeStruct.tm_year = year - YEAR_OFFSET;
	timeStruct.tm_mon = month - MONTH_OFFSET;
	timeStamp = mktime(&timeStruct);

}

string MariaTime::get(string format) {
	char buff[BUFFER_SIZE];
	strftime(buff, BUFFER_SIZE, format.c_str() , localtime(&timeStamp));
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
	int difference = (int)difftime(timeStamp, currentTime);

	if (difference < 0) {
		return STRING_EMPTY;
	}else if (difference > NUM_SECS_PER_MONTH) {
		int value = difference / NUM_SECS_PER_MONTH;
		return formatPlural(value, STRING_MONTH);

	}else if (difference > NUM_SECS_PER_WEEK) {
		int value = difference / NUM_SECS_PER_WEEK;
		return formatPlural(value, STRING_WEEK);

	}else if (difference > NUM_SECS_PER_DAY) {
		int value = difference / NUM_SECS_PER_DAY;
		return formatPlural(value, STRING_DAY);

	} else {
		string returnString;
		if ((difference % NUM_SEC) > 0) {
			returnString += to_string(difference % NUM_SEC) + STRING_SEC;
		}
		difference /= NUM_SEC;
		if ((difference % NUM_MIN) > 0) {
			returnString = to_string(difference % NUM_MIN) + STRING_MIN + returnString;
		}
		difference /= NUM_MIN;
		if ((difference % NUM_HRS) > 0) {
			returnString = to_string(difference % NUM_HRS) + STRING_HOUR + returnString;
		}
		return returnString;
	}

}

MariaTime MariaTime::getCurrentTime() {
	time_t rawtime;
	time ( &rawtime );
	MariaTime toReturn(rawtime);
	return toReturn;
}

string MariaTime::convertToTimeString(MariaTime* time) {
	string returnHour;
	string returnMin;

		if (time->getHour() == 0) {
			returnHour = to_string(time->getHour() + AM_PM_OFFSET);
			returnMin = TIME_AM;
		} else if (time->getHour() <= AM_PM_OFFSET){
			returnHour = to_string(time->getHour());
			returnMin = TIME_AM;
		} else {
			returnHour = to_string(time->getHour() - AM_PM_OFFSET);
			returnMin = TIME_PM;
		}
		returnMin = to_string(time->getMin()) + returnMin;

		if (time->getMin() < DOUBLE_DIGIT_LIMIT) {
				returnMin = STRING_ZERO_PAD + returnMin;
		}

		return returnHour + TIME_SEPARATOR + returnMin;
}

string MariaTime::convertToDateString(MariaTime* time) {
	string toReturn;
	toReturn += to_string(time->getDay()) + DATE_SEPARATOR;
	toReturn += MONTHS[time->getMonth() - MONTH_OFFSET] + DATE_SEPARATOR;
	toReturn += to_string(time->getYear());
	return toReturn;
}

string MariaTime::convertToMonthString(MariaTime* time) {
	string toReturn;
	toReturn += MONTHS[time->getMonth() - MONTH_OFFSET] + DATE_SEPARATOR;
	toReturn += to_string(time->getYear());
	return toReturn;
}

int MariaTime::timeDifference(MariaTime* end, MariaTime* start) {
	return (int)difftime(end->get(), start->get());
}

int MariaTime::compareTo(MariaTime other) {
	double difference = difftime(get(), other.get());
	if (difference > 0){
		return 1;
	}else if (difference < 0){
		return -1;
	}
	return 0;
}

string MariaTime::formatPlural(int value, string text) {
	if (value > 1) {
		return to_string(value) + text + STRING_PLURAL;
	} else {
		return to_string(value) + text;
	}
}