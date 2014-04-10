#include "MariaLogic.h"
#include "MariaUIStateLoading.h"
#include "MariaUIStateHome.h"
#include "MariaUIStateHelp.h"
#include "MariaUIStateShow.h"
#include "MariaUIStateConflict.h"

MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	QApplication::setWindowIcon(QIcon(QString::fromStdString("Resources/marialogo32x32.png")));

	mariaInterpreter = new MariaInterpreter();
	mariaFileManager = new MariaFileManager();
	for(int i = 0; i < 3; i++){
		try{
			mariaTaskManager = new MariaTaskManager(mariaFileManager->openFile());
			break;
		}catch(exception e){
			//todo: do something about failed file
			MessageBox(NULL, L"M.A.R.I.A. is unable to start because its save file is currently being used by another program or user.", L"Error!", MB_OK | MB_ICONERROR);
			quit();
		}
	}
	mariaUI = new MariaUI(this);
	mariaStateManager = new MariaStateManager();

	MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
	mariaStateManager->queueState(STATE_TYPE::LOADING, temp);
	temp->setDisplayText("Loading");
	temp->setLoadingDone();

	//mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHelp((QMainWindow*)mariaUI));

	mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
	mariaUI->getCommandBar()->getTextbox()->setQuestionText("How can I help you?");
	mariaUI->getCommandBar()->getTextbox()->setSuggestText("create Meeting tomorrow for discussion");

	
	_beginthread( &MariaLogic::doShowHideWrapper, 0, this);
}

MariaLogic::~MariaLogic(void) {
	delete mariaFileManager;
	delete mariaTaskManager;
	delete mariaInterpreter;
	delete mariaStateManager;
	delete mariaUI;
}

bool MariaLogic::processUndo() {
	if(mariaStateManager->getCurrentState() != STATE_TYPE::HOME) {
		return false; //temporary disable undo everywhere except home
	}

	MariaStateObject* currentObj = mariaStateManager->getCurrentStateObject();
	MariaTask* changed = mariaTaskManager->undoLast();

	if(changed) {
		mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

		int taskCountDifference = mariaTaskManager->compareToPreviousQuery();
		if(taskCountDifference < 0) {
			//refresh GUI!
			((MariaUIStateHome*)currentObj)->eraseUITask(changed);
		} else if (taskCountDifference > 0) {
			((MariaUIStateHome*)currentObj)->addUITask(changed, MariaUITask::DISPLAY_TYPE::NORMAL);
			((MariaUIStateHome*)currentObj)->setPageEnd();
		}
		mariaTaskManager->compareToPreviousQuery();
		return true;
	}
	return false;
}

bool MariaLogic::checkValidCommand(std::string inputText) {
	return mariaInterpreter->checkValidCommand(inputText);
}

bool MariaLogic::processCommand(std::string inputText) {
	MariaInputObject* input = NULL;
	MariaStateObject* currentObj = mariaStateManager->getCurrentStateObject();

	// Attempt to parse the input.
	try {
		input = mariaInterpreter->parseInput(inputText, mariaStateManager->getCurrentState());
	} catch (exception& e) {
		// Set the question text to be the error message.
		mariaUI->getCommandBar()->getTextbox()->setQuestionText(e.what());
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::UNKNOWN);
		return false;
	}

	// Only empty the input box on a successful command.
	mariaUI->getCommandBar()->getTextbox()->setUserInput("");

	switch (input->getCommandType()) {
		case MariaInputObject::COMMAND_TYPE::EXIT: {
			MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
			mariaStateManager->queueState(STATE_TYPE::LOADING, temp);
			temp->setDisplayText("Good Bye!");
			temp->setLoadingDone();
			temp->setQuitAfterLoadingTrue();
			mariaStateManager->transitState();
		}
		break;

		case MariaInputObject::COMMAND_TYPE::ADD_FLOATING: {
			MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, NULL);
			if (toAdd != NULL) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '" + input->getTitle() + "' has been added!");
				if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME && mariaTaskManager->compareToPreviousQuery()) {
					((MariaUIStateHome*)currentObj)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
				} else if (mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
					((MariaUIStateHome*)currentObj)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::CONTRACTED);
				}
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
				((MariaUIStateHome*)currentObj)->setPageEnd();
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is a problem adding '" + inputText + "'");
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::ADD_DEADLINE: {
			MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, input->getEndTime());
			if (toAdd != NULL) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '" + input->getTitle() + "' has been added!");
				if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME && mariaTaskManager->compareToPreviousQuery()) {
					((MariaUIStateHome*)currentObj)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
				} else if (mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
					((MariaUIStateHome*)currentObj)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::CONTRACTED);
				}
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
				((MariaUIStateHome*)currentObj)->setPageEnd();
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is a problem adding '" + inputText + "'");
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::ADD_TIMED: {
			MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), input->getStartTime(), input->getEndTime());
			if (toAdd != NULL) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '" + input->getTitle() + "' has been added!");
				if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME && mariaTaskManager->compareToPreviousQuery()) {
					((MariaUIStateHome*)currentObj)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
				} else if (mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
					((MariaUIStateHome*)currentObj)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::CONTRACTED);
				}
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
				((MariaUIStateHome*)currentObj)->setPageEnd();
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is a problem adding '" + inputText + "'");
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::EDIT_TITLE: {
			string toEditTitle = input->getTitle();
			if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
				int numberToEdit = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;
				if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
					MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
					toEditTask->getMariaTask()->setTitle(input->getEditField());
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
					
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Ok, I have updated the title.");
					mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
					mariaStateManager->transitState();					
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle, false);

				if (listOfTasks.size() == 1) {
					if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
						listOfTasks[0]->setTitle(input->getEditField());
						mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
						mariaUI->getCommandBar()->getTextbox()->setQuestionText("Ok, I have updated the title.");
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

					mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
					mariaStateManager->transitState();
				}
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::EDIT_START_TIME: {
			string toEditTitle = input->getTitle();
		
			//Jay: Pending splitting up of conflict and non conflict.
			if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
				int numberToEdit = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;
				if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
					MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
					toEditTask->getMariaTask()->setStart(input->getEditTime());
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Consider it done!");
					mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
					mariaStateManager->transitState();
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle, false);

				if (listOfTasks.size() == 1) {
					//Jay: To do, change it to just check if currentObj is a stateDisplay and call updateUI, if not
					//still enable updating but no visual cues.
					if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
						listOfTasks[0]->setStart(input->getEditTime());
						mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
						mariaUI->getCommandBar()->getTextbox()->setQuestionText("Consider it done!");
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

					mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
					mariaStateManager->transitState();
				}
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::EDIT_END_TIME: {
			string toEditTitle = input->getTitle();

			//Jay: Pending removal of check if its in conflict state.
			if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
				int numberToEdit = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;
				if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
					MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
					toEditTask->getMariaTask()->setEnd(input->getEditTime());
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
					
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Consider it done.");
					mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
					mariaStateManager->transitState();
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle, false);

				if (listOfTasks.size() == 1) {
					//Jay: To do, change it to just check if currentObj is a stateDisplay and call updateUI, if not
					//still enable updating but no visual cues.
					if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
						listOfTasks[0]->setEnd(input->getEditTime());
						mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
						mariaUI->getCommandBar()->getTextbox()->setQuestionText("Consider it done!");
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

					mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
					mariaStateManager->transitState();
				}
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::EDIT_DESCRIPTION: {
			string toEditTitle = input->getTitle();

			//Jay: Pending splitting up of conflict and non conflict.
			if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
				int numberToEdit = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;
				if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
					//TO DO, transit to edit state.
					MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
					toEditTask->getMariaTask()->setDescription(input->getEditField());
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
					
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Ok, I have updated the description.");
					mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
					mariaStateManager->transitState();					
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle, false);

				if (listOfTasks.size() == 1) {
					if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME || mariaStateManager->getCurrentState() == STATE_TYPE::SHOW) {
						listOfTasks[0]->setDescription(input->getEditField());
						mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
						mariaUI->getCommandBar()->getTextbox()->setQuestionText("Ok, I have updated the description.");
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

					mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
					mariaStateManager->transitState();
				}
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::SHOW_DATE: {
			// Show today, tomorrow, date
			// Date object is obtained through input->getEndTime();
			//Miki data object cannot be reference. Crashes here.
			MariaTime* startTime = input->getEndTime();
			startTime->setHour(0);
			startTime->setMin(0);
			MariaTime* endTime = input->getEndTime();
			endTime->setHour(23);
			endTime->setMin(59);
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(startTime, endTime);

			mariaUI->getCommandBar()->getTextbox()->setQuestionText("This is what you have on " + MariaTime::convertToDateString(endTime) + ".");
			mariaStateManager->queueState(STATE_TYPE::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, MariaTime::convertToDateString(startTime), listOfTasks));
			mariaStateManager->transitState();

			SAFE_DELETE(startTime);
			SAFE_DELETE(endTime);
		}
		break;

		case MariaInputObject::COMMAND_TYPE::SHOW_ALL: {
			vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks();

			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Sure, here are all the tasks.");
			mariaStateManager->queueState(STATE_TYPE::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, "All Tasks", listOfTasks));
			mariaStateManager->transitState();
		}
		break;

		case MariaInputObject::COMMAND_TYPE::SEARCH: {
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle());

			mariaUI->getCommandBar()->getTextbox()->setQuestionText("This is the result of the search for '" + input->getTitle() + "'.");
			mariaStateManager->queueState(STATE_TYPE::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, input->getTitle(), listOfTasks));
			mariaStateManager->transitState();
		}
		break;

		case MariaInputObject::COMMAND_TYPE::DELETE_TASK: {
			string toDeleteTitle = input->getTitle();

			if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
				int numberToDelete = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;

				if (numberToDelete > 0 && numberToDelete <= tempObj->getTotalUITask()) {
					MariaUITask* toDeleteTask = tempObj->eraseUITask(numberToDelete-1);
					mariaTaskManager->archiveTask(toDeleteTask->getMariaTask());
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Resolved! Anything else?");
					mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
					mariaStateManager->transitState();
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toDeleteTitle,false);

				if (listOfTasks.size() == 1) {
					if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME) {
						mariaTaskManager->archiveTask(listOfTasks[0]);
						mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
						mariaUI->getCommandBar()->getTextbox()->setQuestionText("'" + listOfTasks[0]->getTitle() + "' has been deleted!");
						if(mariaTaskManager->compareToPreviousQuery()) {
							((MariaUIStateHome*)currentObj)->eraseUITask(listOfTasks[0]);
						}
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I remove?");

					mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
					mariaStateManager->transitState();
				}
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::DELETE_ALL: {
			if (mariaStateManager->getCurrentState() == STATE_TYPE::HOME) {
				vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks();
				for (int i = 0; i < listOfTasks.size(); i++ ) {
					mariaTaskManager->archiveTask(listOfTasks[i]);
				}
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

				mariaUI->getCommandBar()->getTextbox()->setQuestionText("All tasks have been deleted.");
				((MariaUIStateHome*)currentObj)->eraseAllUITask();
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::MARK_DONE: {
			string toMarkTitle = input->getTitle();

			if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
				int numberToMark = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;

				if (numberToMark > 0 && numberToMark <= tempObj->getTotalUITask()) {
					MariaUITask* toMarkTask = tempObj->eraseUITask(numberToMark-1);
					toMarkTask->getMariaTask()->setIsDone(true);
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Resolved! Anything else?");
					mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
					mariaStateManager->transitState();
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toMarkTitle);

				if (listOfTasks.size() == 1) {
					if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME) {
						listOfTasks[0]->setIsDone(true);
						mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
						mariaUI->getCommandBar()->getTextbox()->setQuestionText("'" + toMarkTitle + "' has been completed!");
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I mark?");

					mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
					mariaStateManager->transitState();
				}
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::MARK_UNDONE: {
			string toMarkTitle = input->getTitle();

			if (mariaStateManager->getCurrentState() == STATE_TYPE::CONFLICT) {
				int numberToMark = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;

				if (numberToMark > 0 && numberToMark <= tempObj->getTotalUITask()) {
					MariaUITask* toMarkTask = tempObj->eraseUITask(numberToMark-1);
					toMarkTask->getMariaTask()->setIsDone(false);
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Resolved! Anything else?");
					mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
					mariaStateManager->transitState();
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toMarkTitle);

				if (listOfTasks.size() == 1) {
					if(mariaStateManager->getCurrentState() == STATE_TYPE::HOME) {
						listOfTasks[0]->setIsDone(false);
						mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
						mariaUI->getCommandBar()->getTextbox()->setQuestionText("'" + toMarkTitle + "' has been completed!");
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I mark?");

					mariaStateManager->queueState(STATE_TYPE::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, listOfTasks));
					mariaStateManager->transitState();
				}
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::UNDO: {
			if(processUndo()) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Undo was sucessful");
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Nothing to Undo.");
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::GO_HOME: {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("How can I help you?");
			mariaStateManager->queueState(STATE_TYPE::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager->getWeeklyTask()));
			mariaStateManager->transitState();
		}
		break;

		case MariaInputObject::COMMAND_TYPE::PAGE_UP: {
			MariaUIStateDisplay* tempObj = (MariaUIStateDisplay*)currentObj;
			if(tempObj->isAllTaskAtLocation()) {
				if (tempObj->isPageValid(tempObj->getPage()-1)) {
					tempObj->setPage(tempObj->getPage()-1);
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Going up.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are no more items up there.");
				}
			}
		}
		break;

		case MariaInputObject::COMMAND_TYPE::PAGE_DOWN: {
			MariaUIStateDisplay* tempObj = (MariaUIStateDisplay*)currentObj;
			if(tempObj->isAllTaskAtLocation()) {
				if (tempObj->isPageValid(tempObj->getPage()+1)) {
					tempObj->setPage(tempObj->getPage()+1);
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Going down.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are no more items down there.");
				}
			}
		}
		break;
	}

	//Overall UI Refresh
	((MariaUIStateDisplay*)currentObj)->updatePage();

	// Clean up.
	SAFE_DELETE(input);
	currentObj = NULL;

	return true;
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

void __cdecl MariaLogic::doShowHideWrapper(void* mariaLogic) {
	
	static_cast<MariaLogic*>(mariaLogic)->doShowHide();
}

int main(int argc, char *argv[]) {
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtDumpMemoryLeaks();
#endif
	MariaLogic *mariaLogic = new MariaLogic(argc, argv);

	int returnCode = QApplication::exec();
	delete mariaLogic;

	return returnCode;
}
