#include "MariaLogic.h"
#include "MariaUIStateCredits.h"
#include "MariaUIStateLoading.h"
#include "MariaUIStateHome.h"
#include "MariaUIStateHelp.h"
#include "MariaUIStateShow.h"
#include "MariaUIStateConflict.h"

MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	initWindowIcon();
	initShowHideWrapper();

	initComponents();
	initStartingState();
}

MariaLogic::~MariaLogic(void) {
	SAFE_DELETE(mariaFileManager);
	SAFE_DELETE(mariaTaskManager);
	SAFE_DELETE(mariaInterpreter);
	SAFE_DELETE(mariaStateManager);
	SAFE_DELETE(mariaUI);
}

bool MariaLogic::checkValidCommand(std::string inputText) {
	return mariaInterpreter->checkValidCommand(inputText);
}

string MariaLogic::processCommand(std::string inputText) {
	string output = "";
	MariaInputObject* input = NULL;
	MariaStateObject* currentObj = mariaStateManager->getCurrentStateObject();

	try {
		input = mariaInterpreter->parseInput(inputText, mariaStateManager->getCurrentState());
	} catch (exception& e) {
		throw;
	}

	switch (input->getCommandType()) {
		case MariaInputObject::COMMAND_TYPE::EXIT:
			runCommandExit(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::ADD_FLOATING:
			output = runCommandAddFloatingTask(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::ADD_DEADLINE:
			output = runCommandAddDeadlineTask(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::ADD_TIMED:
			output = runCommandAddTimedTask(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::EDIT_TITLE:
			output = runCommandEditTitle(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::EDIT_START_TIME:
			output = runCommandEditStartTime(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::EDIT_END_TIME:
			output = runCommandEditEndTime(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::EDIT_DESCRIPTION:
			output = runCommandEditDescription(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::SHOW_DATE:
			output = runCommandShowDate(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::SHOW_DATE_RANGE:
			runCommandShowDateRange(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::SHOW_ALL:
			output = runCommandShowAll(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::SEARCH:
			output = runCommandSearch(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::DELETE_TASK:
			output = runCommandDeleteTask(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::DELETE_ALL:
			output = runCommandDeleteAll(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::MARK_DONE:
			output = runCommandMarkDone(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::MARK_UNDONE:
			output = runCommandMarkUndone(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::UNDO:
			output = runCommandUndo(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::GO_HOME:
			output = runCommandGoHome(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::GO_CREDITS:
			output = runCommandGoCredits(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::GO_HELP:
			output = runCommandGoHelp(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::PAGE_UP:
			output = runCommandPageUp(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::PAGE_DOWN:
			output = runCommandPageDown(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::PAGE_LEFT:
			output = runCommandPageLeft(input, currentObj);
		break;

		case MariaInputObject::COMMAND_TYPE::PAGE_RIGHT:
			output = runCommandPageRight(input, currentObj);
		break;
	}

	// Rey: Refactoring needed for this line.
	mariaUI->getCommandBar()->getTextbox()->setUserInput("");

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

void MariaLogic::generateTextforUI() {
	if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME) {
		MariaUIPreview *preview = ((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->getUIPreview();

		MariaTime now=MariaTime::getCurrentTime();
		MariaTime startOfDay(now.getYear(),now.getMonth(), now.getDay(), 0, 0);
		MariaTime endOfDay(now.getYear(),now.getMonth(), now.getDay(), 23, 59);
		MariaTime endOfTomorrow(now.getYear(),now.getMonth(), now.getDay()+1, 23, 59);
		MariaTime startOfTomorrow(now.getYear(),now.getMonth(), now.getDay()+1, 0, 0);

		//Generate Task for UIPreview.
		vector<MariaTask*> taskListNow = mariaTaskManager->findTask(&now,&endOfDay, MariaTask::TaskType::TIMED, false);
		vector<MariaTask*> taskListAll = mariaTaskManager->findTask(&startOfDay,&endOfDay, MariaTask::TaskType::TIMED, false);
		vector<MariaTask*> taskListDeadLine = mariaTaskManager->findTask(&now,&endOfDay, MariaTask::TaskType::DEADLINE, false);
		vector<MariaTask*> taskListTomorrow = mariaTaskManager->findTask(&startOfTomorrow, &endOfTomorrow, MariaTask::TaskType::TIMED, false);
		vector<MariaTask*> taskListTomorrowDeadLine = mariaTaskManager->findTask(&startOfTomorrow, &endOfTomorrow, MariaTask::TaskType::DEADLINE, false);
		vector<MariaTask*> taskListSuggest = mariaTaskManager->findTask(MariaTask::TaskType::FLOATING, false);

		//Nearest free day in a week. Day starts from 0. -1 denotes no free day.
		int freeDay = -1;
		for(int i = 0; i < 7 ; i++) {	
			MariaTime startOfFreeDay(now.getYear(),now.getMonth(), now.getDay()+i, 0, 0);
			MariaTime endOfFreeDay(now.getYear(),now.getMonth(), now.getDay()+i, 23, 59);
			vector<MariaTask*> freeDayList = mariaTaskManager->findTask(&startOfFreeDay, &endOfFreeDay, false);
			if(freeDayList.size() == 0) {
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
			MessageBox(NULL, L"M.A.R.I.A. is unable to start because its save file is currently being used by another program or user.", L"Error!", MB_OK | MB_ICONERROR);
			quit();
		}
	}
}

void MariaLogic::initStartingState() {
	MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
	mariaStateManager->queueState(STATE_TYPE::LOADING, temp);
	temp->setDisplayText("Loading");
	temp->setLoadingDone();
	
	mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
}

void MariaLogic::initShowHideWrapper() {
	_beginthread(&MariaLogic::doShowHideWrapper, 0, this);
}

void MariaLogic::runCommandExit(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
	mariaStateManager->queueState(STATE_TYPE::LOADING, temp);
	temp->setDisplayText("Good Bye!");
	temp->setLoadingDone();
	temp->setQuitAfterLoadingTrue();
	mariaStateManager->transitState();
}

string MariaLogic::runCommandAddFloatingTask(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, NULL);
	const char* taskTitle = input->getTitle().c_str();

	if (toAdd != NULL) {
		addTaskToUI(toAdd, state);
		saveToFile();
		return MariaText::format(MariaText::TASK_ADDED_SUCESS, taskTitle);
	} else {
		return MariaText::format(MariaText::TASK_ADDED_ERROR, taskTitle);
	}
}

string MariaLogic::runCommandAddDeadlineTask(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, input->getEndTime());
	const char* taskTitle = input->getTitle().c_str();

	if (toAdd != NULL) {
		addTaskToUI(toAdd, state);
		saveToFile();
		return MariaText::format(MariaText::TASK_ADDED_SUCESS, taskTitle);
	} else {
		return MariaText::format(MariaText::TASK_ADDED_ERROR, taskTitle);
	}
}

string MariaLogic::runCommandAddTimedTask(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), input->getStartTime(), input->getEndTime());
	const char* taskTitle = input->getTitle().c_str();

	if (toAdd != NULL) {
		addTaskToUI(toAdd, state);
		saveToFile();
		return MariaText::format(MariaText::TASK_ADDED_SUCESS, taskTitle);
	} else {
		return MariaText::format(MariaText::TASK_ADDED_ERROR, taskTitle);
	}
}

string MariaLogic::runCommandEditTitle(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
		int numberToEdit = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;
		if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
			MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
			toEditTask->getMariaTask()->setTitle(input->getEditField());
			saveToFile();
					
			mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATED_TITLE;
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
				listOfTasks[0]->setTitle(input->getEditField());
				saveToFile();
				return MariaText::TASK_UPDATED_TITLE;
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {
			mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATE_CONFLICT;
		}
	}
	
	return MariaText::EMPTY_STRING;
}

string MariaLogic::runCommandEditStartTime(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	//Jay: Pending splitting up of conflict and non conflict.
	if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
		int numberToEdit = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;
		if (numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
			MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
			// Check to see if the task even has an start time to begin with.
			if (toEditTask->getMariaTask()->getStart() == NULL) {
				throw exception("Sorry, but please recreate the activity to insert time.");
			}
			// Check to see if the new time is past the end time.
			if (toEditTask->getMariaTask()->getEnd()->compareTo(*input->getEditTime()) < 0) {
				throw exception("Sorry, but the new start time is past the end time.");
			}

			toEditTask->getMariaTask()->setStart(input->getEditTime());
			saveToFile();

			mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return "Consider it done!";
		}
		//rey: possible control path error here!
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			//Jay: To do, change it to just check if currentObj is a stateDisplay and call updateUI, if not
			//still enable updating but no visual cues.
			if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
				// Check to see if the task even has an start time to begin with.
				if (listOfTasks[0]->getStart() == NULL) {
					throw exception("Sorry, but please recreate the activity to insert time.");
				}
				// Check to see if the new time is past the end time.
				if (listOfTasks[0]->getEnd()->compareTo(*input->getEditTime()) < 0) {
					throw exception("Sorry, but the new start time is past the end time.");
				}

				listOfTasks[0]->setStart(input->getEditTime());
				saveToFile();
				return("Consider it done!");
			}
			//rey: possible control path error here!
		} else if (listOfTasks.size() == 0) {
			return ("I couldn't find anything related. Try again.");
		} else {

			mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return ("There are similar tasks, which one should I edit?");
		}
	}
	
	return ("");
}

string MariaLogic::runCommandEditEndTime(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	//Jay: Pending removal of check if its in conflict state.
	if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
		int numberToEdit = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;
		if (numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
			MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
			// Check to see if the task even has an end time to begin with.
			if (toEditTask->getMariaTask()->getEnd() == NULL) {
				throw exception("Sorry, but please recreate the activity to insert time.");
			}
			// Check to see if the new time is before the start time.
			if (toEditTask->getMariaTask()->getStart() != NULL && toEditTask->getMariaTask()->getStart()->compareTo(*input->getEditTime()) > 0) {
				throw exception("Sorry, but the new end time is before the start time.");
			}
			toEditTask->getMariaTask()->setEnd(input->getEditTime());
			saveToFile();
					
			mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return ("Consider it done.");
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			//Jay: To do, change it to just check if currentObj is a stateDisplay and call updateUI, if not
			//still enable updating but no visual cues.
			if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
				// Check to see if the task even has an end time to begin with.
				if (listOfTasks[0]->getEnd() == NULL) {
					throw exception("Sorry, but please recreate the activity to insert time.");
				}
				// Check to see if the new time is before the start time.
				if (listOfTasks[0]->getStart() != NULL && listOfTasks[0]->getStart()->compareTo(*input->getEditTime()) > 0) {
					throw exception("Sorry, but the new end time is before the start time.");
				}
				listOfTasks[0]->setEnd(input->getEditTime());
				saveToFile();
				return ("Consider it done!");
			}
		} else if (listOfTasks.size() == 0) {
			return ("I couldn't find anything related. Try again.");
		} else {

			mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return ("There are similar tasks, which one should I edit?");
		}
	}
	
	return ("");
}

string MariaLogic::runCommandEditDescription(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	//Jay: Pending splitting up of conflict and non conflict.
	if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
		int numberToEdit = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;
		if (numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
			//TO DO, transit to edit state.
			MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
			toEditTask->getMariaTask()->setDescription(input->getEditField());
			saveToFile();
					
			mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();					
			return MariaText::TASK_UPDATED_DESCRIPTION;
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
				listOfTasks[0]->setDescription(input->getEditField());
				saveToFile();
				return MariaText::TASK_UPDATED_DESCRIPTION;
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {

			mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return MariaText::TASK_UPDATE_CONFLICT;
		}
	}
	
	return MariaText::EMPTY_STRING;
}

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

	mariaStateManager->queueState(STATE_TYPE::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, MariaTime::convertToDateString(startTime), listOfTasks));
	mariaStateManager->transitState();

	string toReturn = MariaText::format(MariaText::SHOW_DATE, MariaTime::convertToDateString(endTime));

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

	mariaUI->getCommandBar()->getTextbox()->setQuestionText("This is what you have from " + MariaTime::convertToDateString(startTime) + " to " + MariaTime::convertToDateString(endTime));
	mariaStateManager->queueState(STATE_TYPE::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, MariaTime::convertToDateString(startTime) + " to " + MariaTime::convertToDateString(endTime), listOfTasks));
	mariaStateManager->transitState();

	string toReturn = MariaText::format(MariaText::SHOW_DATE, MariaTime::convertToMonthString(startTime));

	SAFE_DELETE(startTime);
	SAFE_DELETE(endTime);
	return toReturn;
}

string MariaLogic::runCommandShowAll(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks(true);

	mariaStateManager->queueState(STATE_TYPE::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, "All Tasks", listOfTasks));
	mariaStateManager->transitState();
	
	return MariaText::SHOW_ALL;
}

string MariaLogic::runCommandSearch(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), true);
	mariaStateManager->queueState(STATE_TYPE::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, input->getTitle(), listOfTasks));
	mariaStateManager->transitState();

	return MariaText::format(MariaText::SEARCH_RESULT,input->getTitle());
}

string MariaLogic::runCommandDeleteTask(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
		int numberToDelete = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;

		if (numberToDelete > 0 && numberToDelete <= tempObj->getTotalUITask()) {
			MariaUITask* toDeleteTask = tempObj->eraseUITask(numberToDelete-1);
			mariaTaskManager->archiveTask(toDeleteTask->getMariaTask());
			saveToFile();

			mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return ("Resolved! Anything else?");
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle(), false);

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
				mariaTaskManager->archiveTask(listOfTasks[0]);
				saveToFile();
				if (mariaTaskManager->compareToPreviousQuery()) {
					((MariaUIStateHome*)state)->eraseUITask(listOfTasks[0]);
				}
				return ("'" + input->getTitle() + "' has been deleted!");
			}
		} else if (listOfTasks.size() == 0) {
			return MariaText::TASK_NOT_FOUND;
		} else {

			mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return ("There are similar tasks, which one should I remove?");
		}
	}

	return ("");
}

string MariaLogic::runCommandDeleteAll(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
		vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks();
		for (int i = 0; i < listOfTasks.size(); i++ ) {
			mariaTaskManager->archiveTask(listOfTasks[i]);
		}
		saveToFile();

		((MariaUIStateHome*)state)->eraseAllUITask();
		return ("All tasks have been deleted.");
	}

	return ("");
}

string MariaLogic::runCommandMarkDone(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
		int numberToMark = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;

		if (numberToMark > 0 && numberToMark <= tempObj->getTotalUITask()) {
			MariaUITask* toMarkTask = tempObj->eraseUITask(numberToMark-1);
			toMarkTask->getMariaTask()->setIsDone(true);
			saveToFile();

			mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return ("Resolved! Anything else?");
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle());

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
				listOfTasks[0]->setIsDone(true);
				saveToFile();
				return ("'" + input->getTitle() + "' has been completed!");
			}
		} else if (listOfTasks.size() == 0) {
			return ("I couldn't find anything related. Try again.");
		} else {
			mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return ("There are similar tasks, which one should I mark?");
		}
	}

	return ("");
}

string MariaLogic::runCommandMarkUndone(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
		int numberToMark = input->getOptionID();
		MariaUIStateConflict* tempObj = (MariaUIStateConflict*)state;

		if (numberToMark > 0 && numberToMark <= tempObj->getTotalUITask()) {
			MariaUITask* toMarkTask = tempObj->eraseUITask(numberToMark-1);
			toMarkTask->getMariaTask()->setIsDone(false);
			saveToFile();

			mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
			return ("Resolved! Anything else?");
		}
	} else {
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle());

		if (listOfTasks.size() == 1) {
			if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
				listOfTasks[0]->setIsDone(false);
				saveToFile();
				return ("'" + input->getTitle() + "' has been uncompleted!");
			}
		} else if (listOfTasks.size() == 0) {
			return ("I couldn't find anything related. Try again.");
		} else {
			mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
			mariaStateManager->transitState();
			return ("There are similar tasks, which one should I mark?");
		}
	}

	return ("");
}

string MariaLogic::runCommandUndo(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	STATE_TYPE currentState = mariaStateManager->getCurrentState();
	if (currentState != STATE_TYPE::HOME && currentState != STATE_TYPE::SHOW) {
		return "Cannot perform Undo here.";
	}

	MariaStateObject* currentObj = mariaStateManager->getCurrentStateObject();
	MariaTask* changed = mariaTaskManager->undoLast();

	if (changed) {
		saveToFile();

		int taskCountDifference = mariaTaskManager->compareToPreviousQuery();
		if (taskCountDifference < 0) {
			//refresh GUI!
			((MariaUIStateHome*)currentObj)->eraseUITask(changed);
		} else if (taskCountDifference > 0) {
			//((MariaUIStateHome*)currentObj)->addUITask(changed, MariaUITask::DISPLAY_TYPE::NORMAL);
			addTaskToUI(changed, state);
			//((MariaUIStateHome*)currentObj)->setPageEnd();
		} 
		mariaTaskManager->compareToPreviousQuery();
	}
	return ("Nothing to Undo.");
}

string MariaLogic::runCommandGoHome(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	STATE_TYPE currentState = mariaStateManager->getCurrentState();

	switch (currentState) {
	case STATE_TYPE::SHOW:
	case STATE_TYPE::CONFLICT:
		mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
		mariaStateManager->transitState();
		break;

	case STATE_TYPE::HELP:
		((MariaUIStateHelp*)state)->setDoneViewing();
		break;

	case STATE_TYPE::CREDITS:
		((MariaUIStateCredits*)state)->setDoneAnimating();
		break;
	}

	return ("How can I help you?");
}

string MariaLogic::runCommandGoCredits(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	mariaStateManager->queueState(STATE_TYPE::CREDITS, new MariaUIStateCredits((QMainWindow*)mariaUI));
	mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
	mariaStateManager->transitState();

	return ("Sure.");
}

string MariaLogic::runCommandGoHelp(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	mariaStateManager->queueState(STATE_TYPE::HELP, new MariaUIStateHelp((QMainWindow*)mariaUI));
	mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
	mariaStateManager->transitState();

	return ("Let me get you some help.");
}

string MariaLogic::runCommandPageUp(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateDisplay* tempObj = dynamic_cast<MariaUIStateDisplay*>(state);
	if (tempObj != NULL) {
		if (tempObj->isAllTaskAtLocation()) {
			if (tempObj->isPageValid(tempObj->getPage()-1)) {
				tempObj->setPage(tempObj->getPage()-1);
				return ("Going up.");
			} else {
				return ("There are no more items up there.");
			}
		}
	}

	return ("");
}

string MariaLogic::runCommandPageDown(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateDisplay* tempObj = dynamic_cast<MariaUIStateDisplay*>(state);
	if (tempObj != NULL) {
		if (tempObj->isAllTaskAtLocation()) {
			if (tempObj->isPageValid(tempObj->getPage()+1)) {
				tempObj->setPage(tempObj->getPage()+1);
				return ("Going down.");
			} else {
				return ("There are no more items down there.");
			}
		}
	}

	return ("");
}

string MariaLogic::runCommandPageLeft(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateHelp* tempObj = dynamic_cast<MariaUIStateHelp*>(state);

	switch (mariaStateManager->getCurrentState()) {
	case STATE_TYPE::HELP:
		if (tempObj != NULL) {
			tempObj->setHelpIndex(tempObj->getHelpIndex()-1);
		}
		break;

	default:
		SAFE_DELETE(input);
		throw exception("");
		break;
	}

	return ("");
}

string MariaLogic::runCommandPageRight(MariaInputObject* input, MariaStateObject* state) {
	assert(input != NULL);
	assert(state != NULL);

	MariaUIStateHelp* tempObj = dynamic_cast<MariaUIStateHelp*>(state);

	switch (mariaStateManager->getCurrentState()) {
	case STATE_TYPE::HELP:
		if (tempObj != NULL) {
			tempObj->setHelpIndex(tempObj->getHelpIndex()+1);
		}
		break;

	default:
		SAFE_DELETE(input);
		throw exception("");
		break;
	}

	return ("");
}

void MariaLogic::saveToFile() {
	vector<MariaTask*> taskList = mariaTaskManager->getAllTasks();
	mariaFileManager->writeFile(taskList);
}

void MariaLogic::addTaskToUI(MariaTask* toAdd, MariaStateObject* state) {
	if(!mariaTaskManager->compareToPreviousQuery()) {
		vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks(true);
		MariaUIStateShow *nextState = new MariaUIStateShow((QMainWindow*)mariaUI, "All Tasks", listOfTasks);
		int pageNum = getPageNumOfTask(toAdd, listOfTasks);
		nextState->setPage(pageNum);
		mariaStateManager->queueState(STATE_TYPE::SHOW, nextState);
		mariaStateManager->transitState();
	} else if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME) {
		((MariaUIStateHome*)state)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
		((MariaUIStateHome*)state)->setPageEnd();
	} else if (mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
		((MariaUIStateShow*)state)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::CONTRACTED);
		((MariaUIStateShow*)state)->setPageEnd();
	}
}

int MariaLogic::getPageNumOfTask(MariaTask* task, vector<MariaTask*> listOfTasks) {
	for(int i = 0; i<listOfTasks.size(); i++) {
		if(listOfTasks[i] == task) {
			return i/5;
		}
	}
	return listOfTasks.size()/5;
}

void __cdecl MariaLogic::doShowHideWrapper(void* mariaLogic) {
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
	delete mariaLogic;

	return returnCode;
}
