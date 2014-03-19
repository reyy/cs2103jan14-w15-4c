#include <assert.h> 
#include "MariaUIStateDisplay.h"
#include "MariaUI.h"
#include "MariaTaskManager.h"

MariaUIStateDisplay::MariaUIStateDisplay(QMainWindow* qmainWindow, MariaTaskManager *taskManager, float taskStartHeight) : MariaStateObject(qmainWindow) {
	_qmainWindow = qmainWindow;
	_taskManager = taskManager;
	_taskStartHeight = _qmainWindow->height()*taskStartHeight;
	_page = 0;
}

MariaUIStateDisplay::~MariaUIStateDisplay() {
	clearTask();
}

void MariaUIStateDisplay::updateNumber() {
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->setTitlePretext(std::to_string(i + 1) + ". ");
	}
}

MariaUITask* MariaUIStateDisplay::addTask(MariaTask *task, MariaUITask::DISPLAY_TYPE type) {
	MariaUITask *temp = new MariaUITask(_qmainWindow, task, _qmainWindow->width(), type);

	temp->setPosition(QPointF(_qmainWindow->width(), _taskStartHeight + temp->getTaskHeight()*_taskStack.size()));
	temp->setDestination(QPointF(0.0, _taskStartHeight + temp->getTaskHeight()*_taskStack.size()));
	temp->show();
	if(task->getType() != MariaTask::FLOATING) {
		temp->startUpdatingTime();
	}

	_taskStack.push_back(temp);

	return temp;
}

void MariaUIStateDisplay::updateTask() {
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->updateDetails();
		_taskStack.at(i)->setDestination(QPointF(0.0, _taskStartHeight + _taskStack.at(i)->getTaskHeight()*i));
	}
}

void MariaUIStateDisplay::updateTaskDestination(QPointF destination) {
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->setDestination(QPointF(destination.x(), destination.y() + _taskStack.at(i)->getTaskHeight()*i));
	}
}

void MariaUIStateDisplay::updateTaskPosition(QPointF position) {
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->setPosition(QPointF(position.x(), position.y() + _taskStack.at(i)->getTaskHeight()*i));
	}
}

MariaUITask* MariaUIStateDisplay::eraseTask(int index) {
	
	assert(index<_taskStack.size());

	MariaUITask* temp = NULL;
	temp = _taskStack[index];
	_taskDisposeStack.push_back(temp);
	temp->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET, _taskStartHeight + _taskStack.at(index)->getTaskHeight()*index));
	temp->stopUpdatingTime();
	_taskStack.erase(_taskStack.begin() + index);

	updateTask();
	return temp;
}

MariaUITask* MariaUIStateDisplay::eraseTask(MariaTask* task) {
	MariaUITask* temp = NULL;
	for (int i = 0; i < _taskStack.size(); i++ ) {
		if (_taskStack[i]->getMariaTask() == task) {
			temp = _taskStack[i];
			_taskDisposeStack.push_back(temp);
			temp->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET, _taskStartHeight + _taskStack.at(i)->getTaskHeight()*i));
			temp->stopUpdatingTime();
			_taskStack.erase(_taskStack.begin() + i);
		}
	}

	updateTask();
	return temp;
}

void MariaUIStateDisplay::eraseAllTask() {
	for (int i = 0; i < _taskStack.size(); i++ ) {
		_taskStack.at(i)->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET, _taskStartHeight + _taskStack.at(i)->getTaskHeight()*i));
		_taskStack.at(i)->stopUpdatingTime();
	}

	while (_taskStack.size()>0) {
		_taskDisposeStack.push_back(_taskStack.at(0));
		_taskStack.erase(_taskStack.begin());
	}
}

void MariaUIStateDisplay::clearTask() {
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->stopUpdatingTime();
		delete _taskStack.at(i);
	}

	while(_taskStack.size()>0) {
		_taskStack.pop_back();
	}

	for( int i = 0 ; i < _taskDisposeStack.size() ; i++ ) {
		_taskDisposeStack.at(i)->stopUpdatingTime();
		delete _taskDisposeStack.at(i);
	}

	while(_taskDisposeStack.size()>0) {
		_taskDisposeStack.pop_back();
	}
}

int MariaUIStateDisplay::getTotalTask() {
	return _taskStack.size();
}

void MariaUIStateDisplay::setPage(int page) {
	_page = page;
}

int MariaUIStateDisplay::getPage() {
	return _page;
}

void MariaUIStateDisplay::updatePage() {
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET, _taskStartHeight + _taskStack.at(i)->getTaskHeight()*(i-20*_page)));			
	}
}