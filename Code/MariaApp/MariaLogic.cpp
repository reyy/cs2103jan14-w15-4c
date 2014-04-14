#include "MariaLogic.h"
#include "MariaUIStateCredits.h"
#include "MariaUIStateLoading.h"
#include "MariaUIStateHome.h"
#include "MariaUIStateHelp.h"
#include "MariaUIStateShow.h"
#include "MariaUIStateConflict.h"

//@author A0111823U
MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	initWindowIcon();
	initShowHideWrapper();
	initComponents();
	initStartingState();
}

//@author A0111784H
MariaLogic::~MariaLogic(void) {
	SAFE_DELETE(mariaFileManager);
	SAFE_DELETE(mariaTaskManager);
	SAFE_DELETE(mariaInterpreter);
	SAFE_DELETE(mariaStateManager);
	SAFE_DELETE(mariaUI);
}

//@author A0111821X
bool MariaLogic::checkValidCommand(string inputText) {
	return mariaInterpreter->checkValidCommand(inputText);
}

//@author A0111821X
string MariaLogic::processCommand(string inputText) {
	string output = MariaText::EMPTY_STRING;
	MariaInputObject* input = NULL;
	MariaStateObject* currentObj = mariaStateManager->getCurrentStateObject();

	try {
		input = mariaInterpreter->parseInput(inputText, mariaStateManager->getCurrentState());
	} catch (exception& e) {
		throw;
	}

	switch (input->getCommandType()) {
		case MariaInputObject::CommandType::EXIT:
			runCommandExit(input, currentObj);
		break;

		case MariaInputObject::CommandType::ADD_FLOATING:
			output = runCommandAddFloatingTask(input, currentObj);
		break;

		case MariaInputObject::CommandType::ADD_DEADLINE:
			output = runCommandAddDeadlineTask(input, currentObj);
		break;

		case MariaInputObject::CommandType::ADD_TIMED:
			output = runCommandAddTimedTask(input, currentObj);
		break;

		case MariaInputObject::CommandType::EDIT_TITLE:
			output = runCommandEditTitle(input, currentObj);
		break;

		case MariaInputObject::CommandType::EDIT_START_TIME:
			output = runCommandEditStartTime(input, currentObj);
		break;

		case MariaInputObject::CommandType::EDIT_END_TIME:
			output = runCommandEditEndTime(input, currentObj);
		break;

		case MariaInputObject::CommandType::EDIT_DESCRIPTION:
			output = runCommandEditDescription(input, currentObj);
		break;

		case MariaInputObject::CommandType::SHOW_DATE:
			output = runCommandShowDate(input, currentObj);
		break;

		case MariaInputObject::CommandType::SHOW_DATE_RANGE:
			output = runCommandShowDateRange(input, currentObj);
		break;

		case MariaInputObject::CommandType::SHOW_ALL:
			output = runCommandShowAll(input, currentObj);
		break;

		case MariaInputObject::CommandType::SEARCH:
			output = runCommandSearch(input, currentObj);
		break;

		case MariaInputObject::CommandType::DELETE_TASK:
			output = runCommandDeleteTask(input, currentObj);
		break;

		 /*case MariaInputObject::CommandType::DELETE_ALL:
			output = runCommandDeleteAll(input, currentObj);
		break;*/ 

		case MariaInputObject::CommandType::MARK_DONE:
			output = runCommandMarkDone(input, currentObj);
		break;

		case MariaInputObject::CommandType::MARK_UNDONE:
			output = runCommandMarkUndone(input, currentObj);
		break;

		case MariaInputObject::CommandType::UNDO:
			output = runCommandUndo(input, currentObj);
		break;

		case MariaInputObject::CommandType::GO_HOME:
			output = runCommandGoHome(input, currentObj);
		break;

		case MariaInputObject::CommandType::GO_CREDITS:
			output = runCommandGoCredits(input, currentObj);
		break;

		case MariaInputObject::CommandType::GO_HELP:
			output = runCommandGoHelp(input, currentObj);
		break;

		case MariaInputObject::CommandType::PAGE_UP:
			output = runCommandPageUp(input, currentObj);
		break;

		case MariaInputObject::CommandType::PAGE_DOWN:
			output = runCommandPageDown(input, currentObj);
		break;

		case MariaInputObject::CommandType::PAGE_LEFT:
			output = runCommandPageLeft(input, currentObj);
		break;

		case MariaInputObject::CommandType::PAGE_RIGHT:
			output = runCommandPageRight(input, currentObj);
		break;
	}

	mariaUI->getCommandBar()->getTextbox()->setUserInput(MariaText::EMPTY_STRING);

	//Overall UI Refresh
	MariaUIStateDisplay* tempObj = dynamic_cast<MariaUIStateDisplay*>(currentObj);
	if (tempObj != NULL) {
		tempObj->updatePage();
	}

	// Clean up.
	SAFE_DELETE(input);
	currentObj = NULL;

	return output;
}

void MariaLogic::terminateProgram() {
	quit();
}

//@author A0111784H
void MariaLogic::generateTextforUI() {
	if (mariaStateManager->getCurrentState() == StateType::HOME) {
		MariaUIPreview* preview = ((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->getUIPreview();

		MariaTime now=MariaTime::getCurrentTime();
		MariaTime startOfDay(now.getYear(), now.getMonth(), now.getDay(), 0, 0);
		MariaTime endOfDay(now.getYear(), now.getMonth(), now.getDay(), 23, 59);
		MariaTime endOfTomorrow(now.getYear(), now.getMonth(), now.getDay() + 1, 23, 59);
		MariaTime startOfTomorrow(now.getYear(), now.getMonth(), now.getDay() + 1, 0, 0);

		//Generate Task for UIPreview.
		vector<MariaTask*> taskListNow = mariaTaskManager->findTask(&now, &endOfDay, MariaTask::TaskType::TIMED, false);
		vector<MariaTask*> taskListAll = mariaTaskManager->findTask(&startOfDay, &endOfDay, MariaTask::TaskType::TIMED, false);
		vector<MariaTask*> taskListDeadLine = mariaTaskManager->findTask(&now, &endOfDay, MariaTask::TaskType::DEADLINE, false);
		vector<MariaTask*> taskListTomorrow = mariaTaskManager->findTask(&startOfTomorrow, &endOfTomorrow, MariaTask::TaskType::TIMED, false);
		vector<MariaTask*> taskListTomorrowDeadLine = mariaTaskManager->findTask(&startOfTomorrow, &endOfTomorrow, MariaTask::TaskType::DEADLINE, false);
		vector<MariaTask*> taskListSuggest = mariaTaskManager->findTask(MariaTask::TaskType::FLOATING, false);

		//Nearest free day in a week. Day starts from 0. -1 denotes no free day.
		int freeDay = - 1;
		for (int i = 0; i < 7; i++) {	
			MariaTime startOfFreeDay(now.getYear(), now.getMonth(), now.getDay() + i, 0, 0);
			MariaTime endOfFreeDay(now.getYear(), now.getMonth(), now.getDay() + i, 23, 59);
			vector<MariaTask*> freeDayList = mariaTaskManager->findTask(&startOfFreeDay, &endOfFreeDay, false);
			if (freeDayList.size() == 0) {
				freeDay = (now.getDay() + i) % 7;
				break;
			}
		}

		preview->generateTodayText(taskListNow, taskListAll, taskListDeadLine);
		preview->generateTomorrowText(taskListTomorrow, taskListTomorrowDeadLine);
		preview->generateSuggestionText(freeDay, taskListSuggest);
		preview->updateText();
	}
}

//@author generated
void MariaLogic::doShowHide() {
	RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_NOREPEAT, VK_SPACE);
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_HOTKEY) {
			emit mariaUI->triggerShowHideEvent();
		}
	}
}

//@author A0111823U
void MariaLogic::initWindowIcon() {
	QApplication::setWindowIcon(QIcon(QString::fromStdString("Resources/marialogo32x32.png")));
}

void MariaLogic::initComponents() {
	initTaskManager();

	mariaInterpreter = new MariaInterpreter();
	mariaFileManager = new MariaFileManager();
	
	mariaUI = new MariaUI(this);
	mariaStateManager = new MariaStateManager();
}

void MariaLogic::initTaskManager() {
	for (int i = 0; i < 3; i++) {
		try {
			mariaTaskManager = new MariaTaskManager(mariaFileManager->openFile());
			break;
		} catch(exception e) {
			MessageBox(NULL, L"MARIA is unable to start because its save file is currently being used by another program or user.", L"Error!", MB_OK | MB_ICONERROR);
			quit();
		}
	}
}

//@author A0111784H
void MariaLogic::initStartingState() {
	MariaUIStateLoading* temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
	mariaStateManager->queueState(StateType::LOADING, temp);
	temp->setDisplayText(MariaText::LOADING);
	temp->setLoadingDone();
	
	mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
}

//@author A0111823U
void MariaLogic::initShowHideWrapper() {
	_beginthread(&MariaLogic::doShowHideWrapper, 0, this);
}

//@author A0111821X
void MariaLogic::runCommandExit(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateLoading* temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
	mariaStateManager->queueState(StateType::LOADING, temp);
	temp->setDisplayText(MariaText::EXIT);
	temp->setLoadingDone();
	temp->setQuitAfterLoadingTrue();
	mariaStateManager->transitState();
}

//@author A0111823U
string MariaLogic::runCommandAddFloatingTask(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaTask* toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, NULL);
	string taskTitle = input->getTitle();

	if (toAdd != NULL) {
		addTaskToUI(toAdd, state);
		saveToFile();
		return MariaText::format(MariaText::TASK_ADDED_SUCCESS, taskTitle.c_str());
	} else {
		return MariaText::format(MariaText::TASK_ADDED_ERROR, taskTitle.c_str());
	}
}

string MariaLogic::runCommandAddDeadlineTask(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaTask* toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, input->getEndTime());
	string taskTitle = input->getTitle();

	if (toAdd != NULL) {
		addTaskToUI(toAdd, state);
		saveToFile();
		return MariaText::format(MariaText::TASK_ADDED_SUCCESS, taskTitle.c_str());
	} else {
		return MariaText::format(MariaText::TASK_ADDED_ERROR, taskTitle.c_str());
	}
}

//@author A0111821X
string MariaLogic::runCommandAddTimedTask(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaTask* toAdd = mariaTaskManager->addTask(input->getTitle(), input->getStartTime(), input->getEndTime());
	string taskTitle = input->getTitle();

	if (toAdd != NULL) {
		addTaskToUI(toAdd, state);
		saveToFile();
		return MariaText::format(MariaText::TASK_ADDED_SUCCESS, taskTitle.c_str());
	} else {
		return MariaText::format(MariaText::TASK_ADDED_ERROR, taskTitle.c_str());
	}
}

string MariaLogic::runCommandEditTitle(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == StateType::CONFLICT) {
		int numberToEdit = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;
		if (numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
			MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit - 1);
			toEditTask->getMariaTask()->setTitle(input->getEditField());
			saveToFile();
					
			mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATED_TITLE;
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == StateType::HOME || mariaStateManager->getCurrentState() == StateType::SHOW) {
				listOfTasks[0]->setTitle(input->getEditField());
				saveToFile();
				return MariaText::TASK_UPDATED_TITLE;
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {
			mariaStateManager->queueState(StateType::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATE_CONFLICT;
		}
	}
	
	return MariaText::EMPTY_STRING;
}

//@author A0111784H
string MariaLogic::runCommandEditStartTime(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == StateType::CONFLICT) {
		int numberToEdit = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;
		if (numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
			MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit - 1);
			// Check to see if the task even has an start time to begin with.
			if (toEditTask->getMariaTask()->getStart() == NULL) {
				throw MariaText::TASK_UPDATE_TYPE_ERROR;
			}
			// Check to see if the new time is past the end time.
			if (toEditTask->getMariaTask()->getEnd()->compareTo(*input->getEditTime()) < 0) {
				throw MariaText::TASK_UPDATE_START_TIME_ERROR;
			}

			toEditTask->getMariaTask()->setStart(input->getEditTime());
			saveToFile();

			mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATED_TIME;
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == StateType::HOME || mariaStateManager->getCurrentState() == StateType::SHOW) {
				// Check to see if the task even has an start time to begin with.
				if (listOfTasks[0]->getStart() == NULL) {
					throw MariaText::TASK_UPDATE_TYPE_ERROR;
				}
				// Check to see if the new time is past the end time.
				if (listOfTasks[0]->getEnd()->compareTo(*input->getEditTime()) < 0) {
					throw MariaText::TASK_UPDATE_START_TIME_ERROR;
				}

				listOfTasks[0]->setStart(input->getEditTime());
				saveToFile();
				return MariaText::TASK_UPDATED_TIME;
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {

			mariaStateManager->queueState(StateType::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATE_CONFLICT;
		}
	}
	
	return MariaText::EMPTY_STRING;
}

string MariaLogic::runCommandEditEndTime(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == StateType::CONFLICT) {
		int numberToEdit = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;
		if (numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
			MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit - 1);
			// Check to see if the task even has an end time to begin with.
			if (toEditTask->getMariaTask()->getEnd() == NULL) {
				throw MariaText::TASK_UPDATE_TYPE_ERROR;
			}
			// Check to see if the new time is before the start time.
			if (toEditTask->getMariaTask()->getStart() != NULL && toEditTask->getMariaTask()->getStart()->compareTo(*input->getEditTime()) > 0) {
				throw MariaText::TASK_UPDATE_END_TIME_ERROR;
			}
			toEditTask->getMariaTask()->setEnd(input->getEditTime());
			saveToFile();
					
			mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATED_TIME;
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == StateType::HOME || mariaStateManager->getCurrentState() == StateType::SHOW) {
				// Check to see if the task even has an end time to begin with.
				if (listOfTasks[0]->getEnd() == NULL) {
					throw MariaText::TASK_UPDATE_TYPE_ERROR;
				}
				// Check to see if the new time is before the start time.
				if (listOfTasks[0]->getStart() != NULL && listOfTasks[0]->getStart()->compareTo(*input->getEditTime()) > 0) {
					throw MariaText::TASK_UPDATE_END_TIME_ERROR;
				}
				listOfTasks[0]->setEnd(input->getEditTime());
				saveToFile();
				return MariaText::TASK_UPDATED_TIME;
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {

			mariaStateManager->queueState(StateType::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATE_CONFLICT;
		}
	}
	
	return MariaText::EMPTY_STRING;
}

string MariaLogic::runCommandEditDescription(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	//Jay: Pending splitting up of conflict and non conflict.
	if (mariaStateManager->getCurrentState() == StateType::CONFLICT) {
		int numberToEdit = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;
		if (numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
			//TO DO, transit to edit state.
			MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit - 1);
			toEditTask->getMariaTask()->setDescription(input->getEditField());
			saveToFile();
					
			mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();					
			return MariaText::TASK_UPDATED_DESCRIPTION;
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == StateType::HOME || mariaStateManager->getCurrentState() == StateType::SHOW) {
				listOfTasks[0]->setDescription(input->getEditField());
				saveToFile();
				return MariaText::TASK_UPDATED_DESCRIPTION;
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {

			mariaStateManager->queueState(StateType::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATE_CONFLICT;
		}
	}
	
	return MariaText::EMPTY_STRING;
}

//@author A0111821X
string MariaLogic::runCommandShowDate(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaTime* startTime = input->getEndTime();
	startTime->setHour(0);
	startTime->setMin(0);
	MariaTime* endTime = input->getEndTime();
	endTime->setHour(23);
	endTime->setMin(59);
	vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(startTime, endTime, true);

	mariaStateManager->queueState(StateType::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, MariaTime::convertToDateString(startTime), listOfTasks));
	mariaStateManager->transitState();

	string toReturn = MariaText::format(MariaText::SHOW_DATE, MariaTime::convertToDateString(endTime).c_str());

	SAFE_DELETE(startTime);
	SAFE_DELETE(endTime);

	return toReturn;	
}

string MariaLogic::runCommandShowDateRange(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaTime* startTime = input->getStartTime();
	startTime->setHour(0);
	startTime->setMin(0);
	MariaTime* endTime = input->getEndTime();
	endTime->setHour(23);
	endTime->setMin(59);
	vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(startTime, endTime, true);

	string toReturn = MariaText::format(MariaText::SHOW_RANGE, MariaTime::convertToDateString(startTime).c_str(), MariaTime::convertToDateString(endTime).c_str());

	mariaStateManager->queueState(StateType::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, toReturn, listOfTasks));
	mariaStateManager->transitState();

	SAFE_DELETE(startTime);
	SAFE_DELETE(endTime);
	return toReturn;
}

//@author A0111823U
string MariaLogic::runCommandShowAll(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks(true);

	mariaStateManager->queueState(StateType::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, "All Tasks", listOfTasks));
	mariaStateManager->transitState();
	
	return MariaText::SHOW_ALL;
}

string MariaLogic::runCommandSearch(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), true);
	mariaStateManager->queueState(StateType::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, input->getTitle(), listOfTasks));
	mariaStateManager->transitState();

	return MariaText::format(MariaText::SEARCH_RESULT, input->getTitle().c_str());
}

string MariaLogic::runCommandDeleteTask(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == StateType::CONFLICT) {
		int numberToDelete = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;

		if (numberToDelete > 0 && numberToDelete <= tempObj->getTotalUITask()) {
			MariaUITask* toDeleteTask = tempObj->eraseUITask(numberToDelete - 1);
			mariaTaskManager->archiveTask(toDeleteTask->getMariaTask());
			saveToFile();

			mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return MariaText::TASK_RESOLVED_CONFLICT;
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == StateType::HOME || mariaStateManager->getCurrentState() == StateType::SHOW) {
				mariaTaskManager->archiveTask(listOfTasks[0]);
				saveToFile();
				if (mariaTaskManager->compareToPreviousQuery()) {
					((MariaUIStateHome*)state)->eraseUITask(listOfTasks[0]);
				}
				return MariaText::format(MariaText::TASK_DELETED, input->getTitle().c_str());
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {

			mariaStateManager->queueState(StateType::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATE_CONFLICT;
		}
	}

	return MariaText::EMPTY_STRING;
}

//@author generated
string MariaLogic::runCommandDeleteAll(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == StateType::HOME || mariaStateManager->getCurrentState() == StateType::SHOW) {
		vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks();
		for (int i = 0; i < listOfTasks.size(); i++) {
			mariaTaskManager->archiveTask(listOfTasks[i]);
		}
		saveToFile();

		((MariaUIStateHome*)state)->eraseAllUITask();
		return ("All tasks have been deleted.");
	}

	return ("");
}

//@author A0111784H
string MariaLogic::runCommandMarkDone(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == StateType::CONFLICT) {
		int numberToMark = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;

		if (numberToMark > 0 && numberToMark <= tempObj->getTotalUITask()) {
			MariaUITask* toMarkTask = tempObj->eraseUITask(numberToMark - 1);
			toMarkTask->getMariaTask()->setIsDone(true);
			saveToFile();

			mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return MariaText::TASK_RESOLVED_CONFLICT;
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle());

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == StateType::HOME || mariaStateManager->getCurrentState() == StateType::SHOW) {
				listOfTasks[0]->setIsDone(true);
				saveToFile();
				return MariaText::format(MariaText::TASK_MARK_COMPLETE, input->getTitle().c_str());
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {
			mariaStateManager->queueState(StateType::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATE_CONFLICT;
		}
	}

	return MariaText::EMPTY_STRING;
}

string MariaLogic::runCommandMarkUndone(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == StateType::CONFLICT) {
		int numberToMark = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;

		if (numberToMark > 0 && numberToMark <= tempObj->getTotalUITask()) {
			MariaUITask* toMarkTask = tempObj->eraseUITask(numberToMark - 1);
			toMarkTask->getMariaTask()->setIsDone(false);
			saveToFile();

			mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return MariaText::TASK_RESOLVED_CONFLICT;
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle());

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == StateType::HOME || mariaStateManager->getCurrentState() == StateType::SHOW) {
				listOfTasks[0]->setIsDone(false);
				saveToFile();
				return MariaText::format(MariaText::TASK_UNMARK_COMPLETE, input->getTitle().c_str());
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {
			mariaStateManager->queueState(StateType::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATE_CONFLICT;
		}
	}

	return MariaText::EMPTY_STRING;
}

//@author A0111823U
string MariaLogic::runCommandUndo(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	StateType currentState = mariaStateManager->getCurrentState();
	if (currentState != StateType::HOME && currentState != StateType::SHOW) {
		return MariaText::UNDO_ERROR;
	}

	MariaStateObject* currentObj = mariaStateManager->getCurrentStateObject();
	MariaTask* changed = mariaTaskManager->undoLast();

	if (changed) {
		saveToFile();

		int taskCountDifference = mariaTaskManager->compareToPreviousQuery();
		if (taskCountDifference < 0) {
			((MariaUIStateHome*)currentObj)->eraseUITask(changed);
		} else if (taskCountDifference > 0) {
			addTaskToUI(changed, state);
		} 
		mariaTaskManager->compareToPreviousQuery();
		return MariaText::UNDO_SUCCESS;
	}
	return MariaText::UNDO_EMPTY;
}

//@author A0111821X
string MariaLogic::runCommandGoHome(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	StateType currentState = mariaStateManager->getCurrentState();

	switch (currentState) {
	case StateType::SHOW:
	case StateType::CONFLICT:
		mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
		mariaStateManager->transitState();
		break;

	case StateType::HELP:
		((MariaUIStateHelp*)state)->setDoneViewing();
		break;

	case StateType::CREDITS:
		((MariaUIStateCredits*)state)->setDoneAnimating();
		break;
	}

	return MariaText::HOME;
}

string MariaLogic::runCommandGoCredits(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	mariaStateManager->queueState(StateType::CREDITS, new MariaUIStateCredits((QMainWindow*)mariaUI));
	mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
	mariaStateManager->transitState();

	return MariaText::EMPTY_STRING;
}

string MariaLogic::runCommandGoHelp(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	mariaStateManager->queueState(StateType::HELP, new MariaUIStateHelp((QMainWindow*)mariaUI));
	mariaStateManager->queueState(StateType::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
	mariaStateManager->transitState();

	return MariaText::EMPTY_STRING;
}

string MariaLogic::runCommandPageUp(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateDisplay* tempObj = dynamic_cast<MariaUIStateDisplay*>(state);
	if (tempObj != NULL) {
		if (tempObj->isAllTaskAtLocation()) {
			if (tempObj->setPage(tempObj->getPage() - 1)) {
				return MariaText::UP;
			} else {
				return MariaText::UP_ERROR;
			}
		}
	}

	return MariaText::EMPTY_STRING;
}

string MariaLogic::runCommandPageDown(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateDisplay* tempObj = dynamic_cast<MariaUIStateDisplay*>(state);
	if (tempObj != NULL) {
		if (tempObj->isAllTaskAtLocation()) {
			if (tempObj->setPage(tempObj->getPage() + 1)) {
				return MariaText::DOWN;
			} else {
				return MariaText::DOWN_ERROR;
			}
		}
	}

	return MariaText::EMPTY_STRING;
}

//@author A0111784H
string MariaLogic::runCommandPageLeft(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateHelp* tempObj = dynamic_cast<MariaUIStateHelp*>(state);

	switch (mariaStateManager->getCurrentState()) {
	case StateType::HELP:
		if (tempObj != NULL) {
			tempObj->setHelpIndex(tempObj->getHelpIndex() - 1);
		}
		break;

	default:
		SAFE_DELETE(input);
		throw MariaText::IGNORE_EXCEPTION;
		break;
	}

	return MariaText::EMPTY_STRING;
}

string MariaLogic::runCommandPageRight(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateHelp* tempObj = dynamic_cast<MariaUIStateHelp*>(state);

	switch (mariaStateManager->getCurrentState()) {
	case StateType::HELP:
		if (tempObj != NULL) {
			tempObj->setHelpIndex(tempObj->getHelpIndex() + 1);
		}
		break;

	default:
		SAFE_DELETE(input);
		throw MariaText::IGNORE_EXCEPTION;
		break;
	}

	return MariaText::EMPTY_STRING;
}

//@author A0111823U
void MariaLogic::saveToFile() {
	vector<MariaTask*> taskList = mariaTaskManager->getAllTasks();
	mariaFileManager->writeFile(taskList);
}

void MariaLogic::addTaskToUI(MariaTask* toAdd, MariaStateObject* state) {
	if (!mariaTaskManager->compareToPreviousQuery()) {
		vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks(true);
		MariaUIStateShow* nextState = new MariaUIStateShow((QMainWindow*)mariaUI, "All Tasks", listOfTasks);

		int pageNum = getPageNumOfTask(toAdd, listOfTasks, nextState->getMaxTaskDisplay());
		nextState->setPage(pageNum);
		mariaStateManager->queueState(StateType::SHOW, nextState);
		mariaStateManager->transitState();

	} else if (mariaStateManager->getCurrentState() == StateType::HOME) {
		((MariaUIStateHome*)state)->addUITask(toAdd, MariaUITask::DisplayType::NORMAL);
		((MariaUIStateHome*)state)->setPageEnd();

	} else if (mariaStateManager->getCurrentState() == StateType::SHOW) {
		((MariaUIStateShow* )state)->addUITask(toAdd, MariaUITask::DisplayType::DETAILED);
		((MariaUIStateShow* )state)->setPageEnd();
	}
}

int MariaLogic::getPageNumOfTask(MariaTask* task, vector<MariaTask*> listOfTasks, int maxTask) {
	for (int i = 0; i<listOfTasks.size(); i++) {
		if (listOfTasks[i] == task) {
			return i / maxTask;
		}
	}
	return listOfTasks.size() / maxTask;
}

//@author generated
void __cdecl MariaLogic::doShowHideWrapper(void *mariaLogic) {
	static_cast<MariaLogic*>(mariaLogic)->doShowHide();
}

int main(int argc, char *argv[]) {
	// Enable memory leak dumping.
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtDumpMemoryLeaks();
#endif
	MariaLogic *mariaLogic = new MariaLogic(argc, argv);

	int returnCode = QApplication::exec();
	SAFE_DELETE(mariaLogic);

	return returnCode;
}
