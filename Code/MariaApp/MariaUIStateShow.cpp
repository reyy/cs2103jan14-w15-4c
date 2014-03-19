#include <assert.h> 
#include "MariaUIStateShow.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

const float MariaUIStateShow::TASKBAR_STARTHEIGHT_SCALE=0.1;
const float MariaUIStateShow::TASK_STARTHEIGHT_SCALE=0.3;
const float MariaUIStateShow::TASK_STARTHEIGHT_DIFFERENCE=24.0;

MariaUIStateShow::MariaUIStateShow(QMainWindow* qmainWindow,MariaTaskManager *taskManager,VIEW_TYPE currentViewType,MariaTime targetTime) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
	_taskManager=taskManager;
	_currentViewType=currentViewType;
	_targetTime=targetTime;
}

MariaUIStateShow::~MariaUIStateShow() {
	clearTask();
	switch(_currentViewType) {
	case DAY:
		break;
	case MONTH:
		break;
	case YEAR:
		for(int i=0;i<12;i++) {
			delete _monthLabel[i];
		}
		break;
	default:
		break;
	}
	delete _titleLabel;
}

void MariaUIStateShow::initBeginState() {
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(116,30,168);

	_currentTimeLineImage=new QPixmap("./Resources/ui_current_line.png");
	_timeLineImage=new QPixmap("./Resources/ui_line.png");

	_currentTimeLine = new QLabel(_qmainWindow);
	_currentTimeLine->setPixmap(*_currentTimeLineImage);
	_currentTimeLine->setGeometry(QRect(-1000,-1000,12,98));

	_titleLabel = new QLabel(_qmainWindow);
	_titleLabel->setStyleSheet("color:#ffffff; font-size:22px; font-weight:bold;");
	_titleLabel->setAlignment(Qt::AlignCenter);
	switch(_currentViewType) {
	case DAY:
		_titleLabel->setText(MariaTime::DAYS[_targetTime.getDayWeek()]);
		break;
	case MONTH:
		_titleLabel->setText(MariaTime::MONTHS[_targetTime.getMonth()]);
		break;
	case YEAR:
		_titleLabel->setText(QString::number(_targetTime.getYear()));

		for(int i=0;i<12;i++) {
			_monthLabel[i] = new QLabel(_qmainWindow);
			_monthLabel[i]->setStyleSheet("border: 1px solid black; color:#ffffff; font-size:12px; font-weight:bold;");
			_monthLabel[i]->setAlignment(Qt::AlignHCenter);
			_monthLabel[i]->setText(MariaTime::MONTHS[i]);
			_monthLabel[i]->show();
		}
		break;
	default:
		_titleLabel->setText("");
		break;
	}
	_titleLabel->show();
}

void MariaUIStateShow::initActiveState() {
	vector<MariaTask*> tempList = _taskManager->findTask("");
	for(MariaTask* temp : tempList){
		addTask(temp);
	}
	sortTask();
	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->setPosition(QPointF(_qmainWindow->width(),_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
		_taskStack.at(i)->setDestination(QPointF(0.0,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
	}
}

void MariaUIStateShow::initEndState() {
}

bool MariaUIStateShow::timerBeginState() {
	_titleLabel->setGeometry(QRect(getPosition().x()+_qmainWindow->width()*0.5-60,getPosition().y()+80,120,98));
	
	switch(_currentViewType) {
	case DAY:
		break;
	case MONTH:
		break;
	case YEAR:
		for(int j=0;j<4;j++) {
			for(int i=0;i<3;i++) {
				_monthLabel[i+j*3]->setGeometry(QRect(getPosition().x()+i*140+100-70,getPosition().y()+j*110+140,140+1,110+1));
			}
		}
		break;
	default:
		break;
	}
	return false;
}

bool MariaUIStateShow::timerActiveState() {
	return false;
}

bool MariaUIStateShow::timerEndState() {
	_titleLabel->setGeometry(QRect(getPosition().x()+_qmainWindow->width()*0.5-60,getPosition().y()+80,120,98));
	
	switch(_currentViewType) {
	case DAY:
		break;
	case MONTH:
		break;
	case YEAR:
		for(int j=0;j<4;j++) {
			for(int i=0;i<3;i++) {
				_monthLabel[i+j*3]->setGeometry(QRect(getPosition().x()+i*140+100-70,getPosition().y()+j*110+140,140+1,110+1));
			}
		}
		break;
	default:
		break;
	}
	return false;
}

bool MariaUIStateShow::sortTaskFunction(MariaUITask *i,MariaUITask *j) {

	MariaTask *a=i->getMariaTask();
	MariaTask *b=j->getMariaTask();
	if(a->getType()==b->getType()) {

		return false;
	} else {
		return (a->getType()<b->getType());
	}
}

MariaUITask* MariaUIStateShow::addTask(MariaTask *task) {
	MariaUITask *temp = new MariaUITask(_qmainWindow,task,_qmainWindow->width(),MariaUITask::DISPLAY_TYPE::NORMAL);

	temp->setPosition(QPointF(_qmainWindow->width(),_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*_taskStack.size()));
	if(_taskStack.size()>MAX_ITEM_IN_PAGE) {
		temp->setDestination(QPointF(0.0,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*_taskStack.size()+_qmainWindow->height()));
	} else {
		temp->setDestination(QPointF(0.0,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*_taskStack.size()));
	}
	temp->show();
	if(task->getType()!=MariaTask::FLOATING) {
		temp->startUpdatingTime();
	}

	_taskStack.push_back(temp);

	return temp;
}

void MariaUIStateShow::updateTask() {
	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->updateDetails();
		_taskStack.at(i)->setDestination(QPointF(0.0,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
	}
}

MariaUITask* MariaUIStateShow::eraseTask(int index) {
	
	assert(index<_taskStack.size());

	MariaUITask* temp = NULL;
	temp = _taskStack[index];
	_taskDisposeStack.push_back(temp);
	temp->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*index));
	temp->stopUpdatingTime();
	_taskStack.erase(_taskStack.begin()+index);

	sortTask();
	return temp;
}

MariaUITask* MariaUIStateShow::eraseTask(MariaTask* task) {
	MariaUITask* temp = NULL;
	for (int i = 0; i < _taskStack.size(); i++) {
		if (_taskStack[i]->getMariaTask() == task) {
			temp = _taskStack[i];
			_taskDisposeStack.push_back(temp);
			temp->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
			temp->stopUpdatingTime();
			_taskStack.erase(_taskStack.begin()+i);
		}
	}

	sortTask();
	return temp;
}

void MariaUIStateShow::eraseAllTask() {
	for (int i = 0; i < _taskStack.size(); i++) {
		_taskStack.at(i)->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
		_taskStack.at(i)->stopUpdatingTime();
	}

	while (_taskStack.size()>0) {
		_taskDisposeStack.push_back(_taskStack.at(0));
		_taskStack.erase(_taskStack.begin());
	}
}

void MariaUIStateShow::clearTask() {
	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->stopUpdatingTime();
		delete _taskStack.at(i);
	}

	while(_taskStack.size()>0) {
		_taskStack.pop_back();
	}

	for(int i=0;i<_taskDisposeStack.size();i++) {
		_taskDisposeStack.at(i)->stopUpdatingTime();
		delete _taskDisposeStack.at(i);
	}

	while(_taskDisposeStack.size()>0) {
		_taskDisposeStack.pop_back();
	}
}

void MariaUIStateShow::sortTask() {
	std::sort (_taskStack.begin(), _taskStack.end(), MariaUIStateShow::sortTaskFunction);
	updateTask();
}

int MariaUIStateShow::getTotalTask() {
	return _taskStack.size();
}

void MariaUIStateShow::setPage(int page) {
	_page=page;
}

int MariaUIStateShow::getPage() {
	return _page;
}

void MariaUIStateShow::updatePage() {

	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*(i-MAX_ITEM_IN_PAGE*_page)));			
	}
}