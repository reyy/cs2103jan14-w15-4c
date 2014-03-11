#include "MariaUIStateHome.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

const float MariaUIStateHome::TASKBAR_STARTHEIGHT_SCALE=0.25;
const float MariaUIStateHome::TASK_STARTHEIGHT_SCALE=0.35;
const float MariaUIStateHome::TASK_STARTHEIGHT_DIFFERENCE=16.0;

MariaUIStateHome::MariaUIStateHome(MariaTaskManager *taskManager,QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
	_taskManager=taskManager;

	_clock=new MariaUIClock(_qmainWindow);
}

MariaUIStateHome::~MariaUIStateHome() {
	clearTask();
	delete _clock;
}

void MariaUIStateHome::initBeginState() {
	_clock->startUpdating();
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(255,127,39);
}

void MariaUIStateHome::initActiveState() {
	vector<MariaTask*> tempList = _taskManager->findTask("");
	for(MariaTask* temp : tempList){
		addTask(temp);
	}
	sortTask();
}

void MariaUIStateHome::initEndState() {

	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->setDestination(QPointF(-_qmainWindow->width(),_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
	}
}

bool MariaUIStateHome::timerBeginState() {
	_clock->updateGUI(getPosition());
	return false;
}

bool MariaUIStateHome::timerActiveState() {
	return false;
}

bool MariaUIStateHome::timerEndState() {
	_clock->updateGUI(getPosition());
	return false;
}

//Function determines which task shall be sorted as smaller.
//Deadline task shall take precedence in the sort.
//Followed by dateline task. Then floating task.
bool MariaUIStateHome::sortTaskFunction(MariaUITask *i,MariaUITask *j) {

	MariaTask *a=i->getMariaTask();
	MariaTask *b=j->getMariaTask();
	if(a->getType()==b->getType()) {

		return false;
	} else {
		return (a->getType()<b->getType());
	}
}

MariaUITask* MariaUIStateHome::addTask(MariaTask *task) {
	MariaUITask *temp = new MariaUITask(_qmainWindow,task,_qmainWindow->width()-TEXTBOX_X_OFFSET*2);

	temp->setPosition(QPointF(_qmainWindow->width(),_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*_taskStack.size()));
	temp->setDestination(QPointF(TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*_taskStack.size()));
	temp->show();

	_taskStack.push_back(temp);
	temp->setTitlePretext(std::to_string(_taskStack.size())+". ");

	return temp;
}

MariaUITask* MariaUIStateHome::eraseTask(int index) {
	MariaUITask *temp=_taskStack.at(index);
	_taskStack.erase(_taskStack.begin()+index);

	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->setTitlePretext(std::to_string(i+1)+". ");
		_taskStack.at(i)->setDestination(QPointF(TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
	}
	return temp;
}

MariaUITask* MariaUIStateHome::eraseTask(MariaTask* task) {
	MariaUITask* temp = NULL;
	for (int i = 0; i < _taskStack.size(); i++) {
		if (_taskStack[i]->getMariaTask() == task) {
			temp = _taskStack[i];
			_taskStack.erase(_taskStack.begin()+i);
		}
	}

	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->setTitlePretext(std::to_string(i+1)+". ");
		_taskStack.at(i)->setDestination(QPointF(TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
	}
	return temp;
}

void MariaUIStateHome::clearTask() {
	for(int i=0;i<_taskStack.size();i++) {
		delete _taskStack.at(i);
	}

	while(_taskStack.size()>0) {
		_taskStack.pop_back();
	}
}

void MariaUIStateHome::sortTask() {
	std::sort (_taskStack.begin(), _taskStack.end(), MariaUIStateHome::sortTaskFunction);
	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->setTitlePretext(std::to_string(i+1)+". ");
		_taskStack.at(i)->setDestination(QPointF(TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
	}
}