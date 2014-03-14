#include <assert.h> 
#include "MariaUIStateConflict.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

const float MariaUIStateConflict::TASKBAR_STARTHEIGHT_SCALE=0.1;
const float MariaUIStateConflict::TASK_STARTHEIGHT_SCALE=0.2;
const float MariaUIStateConflict::TASK_STARTHEIGHT_DIFFERENCE=154.0;

MariaUIStateConflict::MariaUIStateConflict(string conflictTaskTitle, MariaTaskManager *taskManager,QMainWindow* qmainWindow) : MariaStateObject(qmainWindow) {
	_qmainWindow=qmainWindow;
	_taskManager=taskManager;

	_conflictTaskTitle=conflictTaskTitle;
}

MariaUIStateConflict::~MariaUIStateConflict() {
	clearTask();
}

void MariaUIStateConflict::initBeginState() {
	clearTask();
	((MariaUI*)_qmainWindow)->getCommandBar()->setDestination(_qmainWindow->height()*TASKBAR_STARTHEIGHT_SCALE);
	((MariaUI*)_qmainWindow)->setBackgroundColor(255,0,0);
}

void MariaUIStateConflict::initActiveState() {
	vector<MariaTask*> tempList = _taskManager->findTask(_conflictTaskTitle);
	for(MariaTask* temp : tempList){
		addTask(temp);
	}
	sortTask();
	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->setPosition(QPointF(_qmainWindow->width(),_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
		_taskStack.at(i)->setDestination(QPointF(0.0,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
	}
}

void MariaUIStateConflict::initEndState() {

	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
	}
}

bool MariaUIStateConflict::timerBeginState() {
	return false;
}

bool MariaUIStateConflict::timerActiveState() {
	return false;
}

bool MariaUIStateConflict::timerEndState() {
	return false;
}

//Function determines which task shall be sorted as smaller.
//Deadline task shall take precedence in the sort.
//Followed by dateline task. Then floating task.
bool MariaUIStateConflict::sortTaskFunction(MariaUITaskExpanded *i,MariaUITaskExpanded *j) {

	MariaTask *a=i->getMariaTask();
	MariaTask *b=j->getMariaTask();
	if(a->getType()==b->getType()) {

		return false;
	} else {
		return (a->getType()<b->getType());
	}
}

void MariaUIStateConflict::updateNumber() {
	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->setTitlePretext(std::to_string(i+1)+". ");
	}
}

MariaUITaskExpanded* MariaUIStateConflict::addTask(MariaTask *task) {
	MariaUITaskExpanded *temp = new MariaUITaskExpanded(_qmainWindow,task,_qmainWindow->width());

	temp->setPosition(QPointF(_qmainWindow->width(),_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*_taskStack.size()));
	temp->setDestination(QPointF(0.0,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*_taskStack.size()));
	temp->show();

	_taskStack.push_back(temp);

	return temp;
}

void MariaUIStateConflict::updateTask() {
	for(int i=0;i<_taskStack.size();i++) {
		_taskStack.at(i)->updateDetails();
		_taskStack.at(i)->setDestination(QPointF(0.0,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
	}
}

MariaUITaskExpanded* MariaUIStateConflict::eraseTask(int index) {
	
	assert(index<_taskStack.size());

	MariaUITaskExpanded* temp = NULL;
	temp = _taskStack[index];
	_taskDisposeStack.push_back(temp);
	temp->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*index));
	_taskStack.erase(_taskStack.begin()+index);

	sortTask();
	return temp;
}

MariaUITaskExpanded* MariaUIStateConflict::eraseTask(MariaTask* task) {
	MariaUITaskExpanded* temp = NULL;
	for (int i = 0; i < _taskStack.size(); i++) {
		if (_taskStack[i]->getMariaTask() == task) {
			temp = _taskStack[i];
			_taskDisposeStack.push_back(temp);
			temp->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET,_qmainWindow->height()*TASK_STARTHEIGHT_SCALE+TASK_STARTHEIGHT_DIFFERENCE*i));
			_taskStack.erase(_taskStack.begin()+i);
		}
	}

	sortTask();
	return temp;
}

void MariaUIStateConflict::clearTask() {
	for(int i=0;i<_taskStack.size();i++) {
		delete _taskStack.at(i);
	}

	while(_taskStack.size()>0) {
		_taskStack.pop_back();
	}

	for(int i=0;i<_taskDisposeStack.size();i++) {
		delete _taskDisposeStack.at(i);
	}

	while(_taskDisposeStack.size()>0) {
		_taskDisposeStack.pop_back();
	}
}

void MariaUIStateConflict::sortTask() {
	std::sort (_taskStack.begin(), _taskStack.end(), MariaUIStateConflict::sortTaskFunction);
	updateTask();
	updateNumber();
}

int MariaUIStateConflict::getTotalTask() {
	return _taskStack.size();
}