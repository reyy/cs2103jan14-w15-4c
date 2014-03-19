#include "MariaUIClock.h"
#include "MariaTime.h"

const float MariaUIClock::TIME_WIDTH = 200;
const float MariaUIClock::TIME_HEIGHT = 60;
const float MariaUIClock::TIME_X_OFFSET = -5;
const float MariaUIClock::TIME_Y_OFFSET = 30;
const float MariaUIClock::DATE_WIDTH = 200;
const float MariaUIClock::DATE_HEIGHT = 30;
const float MariaUIClock::DATE_X_OFFSET = 5;
const float MariaUIClock::DATE_Y_OFFSET = 60;
const float MariaUIClock::DAY_WIDTH = 200;
const float MariaUIClock::DAY_HEIGHT = 30;
const float MariaUIClock::DAY_X_OFFSET = 5;
const float MariaUIClock::DAY_Y_OFFSET = 40;

MariaUIClock::MariaUIClock(QMainWindow *qmainWindow) {
	_qmainWindow = qmainWindow;

	_currentTime = new QLabel(_qmainWindow);
	_currentTime->setAlignment(Qt::AlignRight);
	_currentTime->setStyleSheet("color:#ffffff; font-size:50px;");
	_currentTime->hide();
	_currentDate = new QLabel(_qmainWindow);
	_currentDate->setAlignment(Qt::AlignLeft);
	_currentDate->setStyleSheet("color:#ffffff; font-size:22px; font-weight:bold;");
	_currentDate->hide();
	_currentDay = new QLabel(_qmainWindow);
	_currentDay->setAlignment(Qt::AlignLeft);
	_currentDay->setStyleSheet("color:#ffffff; font-size:22px; font-weight:bold;");
	_currentDay->hide();

	_clockTimer = new QTimer(this);
 connect(_clockTimer, SIGNAL(timeout()), this, SLOT(updateClock()));
}

MariaUIClock::~MariaUIClock(void) {
	if(_clockTimer->isActive())
		_clockTimer->stop();
	
	delete _currentDay;
	delete _currentDate;
	delete _currentTime;
	delete _clockTimer;
}

void MariaUIClock::updateClock() {
	
	MariaTime currentTime = MariaTime::getCurrentTime();
	QString tempString;

	//Set Time and Date
	if(currentTime.getHour()%12 == 0) {
		tempString = QString::number(12);
	} else {
		tempString = QString::number(currentTime.getHour()%12);
	}
	if(currentTime.getMin()<10) {
		tempString += ":0" + QString::number(currentTime.getMin());
	} else {
		tempString += ":" + QString::number(currentTime.getMin());
	}

	if(currentTime.getHour()<12) {
		tempString += "AM";
	} else {
		tempString += "PM";
	}
	_currentTime->setText(tempString);
	_currentDate->setText(QString::number(currentTime.getDay()) + " " + QString(MariaTime::MONTHS[currentTime.getMonth()-1]) + " " + QString::number(currentTime.getYear()));
	_currentDay->setText(QString(MariaTime::DAYS[currentTime.getDayWeek()]));
}

void MariaUIClock::startUpdating() {
	if(!_clockTimer->isActive()) {
		_clockTimer->start(1000);
		_currentTime->show();
		_currentDate->show();
		_currentDay->show();
		updateClock();
	}
}

void MariaUIClock::updateGUI(QPointF statePosition) {
	_currentTime->setGeometry(QRect(statePosition.x() + _qmainWindow->width()*0.5-TIME_WIDTH + TIME_X_OFFSET, 
		statePosition.y() + TIME_Y_OFFSET, TIME_WIDTH, TIME_HEIGHT));
	_currentDate->setGeometry(QRect(statePosition.x() + _qmainWindow->width()*0.5 + DATE_X_OFFSET, 
		statePosition.y() + DATE_Y_OFFSET, DATE_WIDTH, DATE_HEIGHT));
	_currentDay->setGeometry(QRect(statePosition.x() + _qmainWindow->width()*0.5 + DAY_X_OFFSET, 
		statePosition.y() + DAY_Y_OFFSET, DAY_WIDTH, DAY_HEIGHT));
}