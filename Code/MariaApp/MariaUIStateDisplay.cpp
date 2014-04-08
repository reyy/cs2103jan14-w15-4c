#include <assert.h> 
#include "MariaUIStateDisplay.h"
#include "MariaUI.h"

const float MariaUIStateDisplay::TITLE_Y_OFFSET = 4.0;
const float MariaUIStateDisplay::TITLE_WIDTH = 200.0;
const float MariaUIStateDisplay::TITLE_HEIGHT = 40.0;
const float MariaUIStateDisplay::FONT_SIZE = 12.0;

MariaUIStateDisplay::MariaUIStateDisplay(QMainWindow* qmainWindow, float taskStartHeight, int maxTaskDisplay) : MariaStateObject(qmainWindow) {
	_qmainWindow = qmainWindow;
	_taskStartHeight = _qmainWindow->height()*taskStartHeight;
	_page = 0;
	_maxTaskDisplay = maxTaskDisplay;
	
	_pageText = new QLabel(_qmainWindow);
	_pageText->setStyleSheet("color:#ffffff; font-size:" + QString::number(FONT_SIZE) + "px;");
	_pageText->setAlignment(Qt::AlignCenter);
	_pageText->hide();
}

MariaUIStateDisplay::~MariaUIStateDisplay() {
	clearUITask();
	delete _pageText;
}

void MariaUIStateDisplay::updateUITaskNumber() {

	int maxShown = _taskStack.size() - _page*_maxTaskDisplay;
	if(maxShown > _maxTaskDisplay) {
		maxShown = _maxTaskDisplay;
	}

	for( int i = _page*_maxTaskDisplay ; i < _page*_maxTaskDisplay + maxShown ; i++ ) {
		_taskStack.at(i)->setTitlePretext(std::to_string(i + 1) + ". ");
	}
}

MariaUITask* MariaUIStateDisplay::addUITask(MariaTask *task, MariaUITask::DISPLAY_TYPE type) {
	int currentPosition = ((int)_taskStack.size()) - _page*_maxTaskDisplay;

	int range = ((int)_taskStack.size()) - _page*_maxTaskDisplay;
	if(range > _maxTaskDisplay) {
		range = _maxTaskDisplay;
	}
	
	float compoundPosition = 0.0;
	for( int i = _page*_maxTaskDisplay ; i < _page*_maxTaskDisplay + range ; i++ ) {
		compoundPosition += _taskStack.at(i)->getTaskHeight();
	}

	float yOffset = 0.0;
	if(currentPosition < 0) {
		yOffset -= _qmainWindow->height();
	}
	if(currentPosition >= _maxTaskDisplay) {
		yOffset += _qmainWindow->height();
	}

	MariaUITask *temp = new MariaUITask(_qmainWindow, task, _qmainWindow->width(), type);
	
	if(currentPosition >= 0 && currentPosition < _maxTaskDisplay) {
		temp->setPosition(QPointF(_qmainWindow->width(), _taskStartHeight + compoundPosition + yOffset));
		temp->setDestination(QPointF(0.0, _taskStartHeight + compoundPosition + yOffset));
		temp->activate();
	} else {
		temp->setPosition(QPointF(0.0, _taskStartHeight + yOffset));
		temp->setDestination(QPointF(0.0, _taskStartHeight + yOffset));
	}

	if(task->getType() != MariaTask::FLOATING) {
		temp->startUpdatingTime();
	}

	_taskStack.push_back(temp);
	
	//Push the page to the last to show the new item added.
//	setPageEnd();
	updatePage();
	updateGUI();
	
	return temp;
}

void MariaUIStateDisplay::updateUITask() {
	float compoundPosition = 0.0;
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->updateDetails();
		_taskStack.at(i)->setDestination(QPointF(0.0, _taskStartHeight + compoundPosition));
		compoundPosition += _taskStack.at(i)->getTaskHeight();
	}

	updateGUI();
}

void MariaUIStateDisplay::updateUITaskDestination(QPointF destination) {
	float compoundPosition = 0.0;
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->setDestination(QPointF(destination.x(), destination.y() + compoundPosition));
		compoundPosition += _taskStack.at(i)->getTaskHeight();
	}
}

void MariaUIStateDisplay::updateUITaskPosition(QPointF position) {
	float compoundPosition = 0.0;
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->setPosition(QPointF(position.x(), position.y() + compoundPosition));
		compoundPosition += _taskStack.at(i)->getTaskHeight();
	}
}

MariaUITask* MariaUIStateDisplay::eraseUITask(int index) {
	
	assert(index<_taskStack.size());

	MariaUITask* temp = NULL;
	temp = _taskStack[index];
	_taskDisposeStack.push_back(temp);
	temp->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET, temp->getPosition().y()));
	temp->stopUpdatingTime();
	_taskStack.erase(_taskStack.begin() + index);

	setPage(0);
	updateTitleText();
	updateUITask();
	updatePage();
	updateGUI();
	return temp;
}

MariaUITask* MariaUIStateDisplay::eraseUITask(MariaTask* task) {
	MariaUITask* temp = NULL;
	for (int i = 0; i < _taskStack.size(); i++ ) {
		if (_taskStack[i]->getMariaTask() == task) {
			temp = _taskStack[i];
			_taskDisposeStack.push_back(temp);
			temp->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET, _taskStack.at(i)->getPosition().y()));
			temp->stopUpdatingTime();
			_taskStack.erase(_taskStack.begin() + i);
			setPage(i/_maxTaskDisplay);
			break;
		}
	}
	
	updateTitleText();
	updateUITask();
	updatePage();
	updateGUI();
	return temp;
}

void MariaUIStateDisplay::eraseAllUITask() {
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {
		_taskStack.at(i)->setDestination(QPointF(-_qmainWindow->width()-TEXTBOX_X_OFFSET, _taskStack.at(i)->getPosition().y()));
		_taskStack.at(i)->stopUpdatingTime();
	}

	while (_taskStack.size()>0) {
		_taskDisposeStack.push_back(_taskStack.at(0));
		_taskStack.erase(_taskStack.begin());
	}
	
	setPage(0);
	updateTitleText();
	updatePage();
	updateGUI();
}

void MariaUIStateDisplay::clearUITask() {
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

	updateTitleText();
	updatePage();
	updateGUI();
}

int MariaUIStateDisplay::getTotalUITask() {
	return _taskStack.size();
}

void MariaUIStateDisplay::setPage(int page) {
	_page = page;
}

void MariaUIStateDisplay::setPageEnd() {
	if(getTotalUITask() == 0) {
		_page = 0;
	} else {
		_page = ((getTotalUITask()-1) / _maxTaskDisplay);
	}
}

int MariaUIStateDisplay::getPage() {
	return _page;
}

bool MariaUIStateDisplay::isPageValid(int page) {
	if(page >= 0 && page <= ceil((getTotalUITask()-1) / _maxTaskDisplay)) {
		return true;
	} else {
		return false;
	}
}

void MariaUIStateDisplay::updatePage() {
	float compoundPosition = 0.0;
	for( int i = 0 ; i < _taskStack.size() ; i++ ) {

		int currentPosition = i - _page*_maxTaskDisplay;
		float yOffset = 0.0;
		if(currentPosition < 0) {
			yOffset -= _qmainWindow->height();
		}
		if(currentPosition >= _maxTaskDisplay) {
			yOffset += _qmainWindow->height();
		}
		
		if(yOffset == 0.0) {
			_taskStack.at(i)->setDestination(QPointF(0.0, _taskStartHeight + compoundPosition + yOffset));
			_taskStack.at(i)->activate();
			compoundPosition += _taskStack.at(i)->getTaskHeight();
		} else {
			_taskStack.at(i)->setDestination(QPointF(0.0, _taskStartHeight + yOffset));
		}
	}
	updateTitleText();
}

void MariaUIStateDisplay::updateTitleText() {
	int offsetBeforePage = _page*_maxTaskDisplay;
	int offsetAfterPage = ((int)_taskStack.size() - (_page+1)*_maxTaskDisplay);
	

	string toShow="";
	if(offsetBeforePage > 0) {
		toShow += to_string(offsetBeforePage);
		if(offsetBeforePage > 1) {
			toShow +=  + " items up above";
		} else {
			toShow +=  + " item up above";
		}
	}

	toShow += "\n";

	if(offsetAfterPage > 0) {
		toShow += to_string(offsetAfterPage);
		if(offsetBeforePage > 1) {
			toShow +=  + " items down below";
		} else {
			toShow +=  + " item down below";
		}
	}

	_pageText->setText(QString::fromStdString(toShow));
}

void MariaUIStateDisplay::updatePageTitleGUI() {
	_pageText->show();
	_pageText->setGeometry(QRect(getPosition().x() + _qmainWindow->width()*0.5 - TITLE_WIDTH*0.5, getPosition().y() + _qmainWindow->height() - TITLE_Y_OFFSET - TITLE_HEIGHT, TITLE_WIDTH, TITLE_HEIGHT));
}

void MariaUIStateDisplay::updateGUI() {

}