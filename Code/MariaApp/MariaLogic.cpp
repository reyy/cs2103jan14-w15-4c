#include "MariaLogic.h"
#include "MariaUIStatus.h"
#include "MariaUIStateLoading.h"
#include "MariaUIStateHome.h"
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
	mariaStateManager->queueState(MariaStateManager::LOADING, temp);
	temp->setDisplayText("Loading");
	temp->setLoadingDone();


	mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
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
	if(mariaTaskManager->undoLast()) {
		mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
		if(typeid(mariaStateManager->getCurrentStateObject()) == typeid(MariaUIStateDisplay)) {
			((MariaUIStateDisplay*)(mariaStateManager->getCurrentStateObject()))->updateUITask();
		}
		return true;
	}
	return false;
}

bool MariaLogic::processCommand(std::string inputText) {
	MariaInputObject* input = mariaInterpreter->parseInput(inputText, mariaStateManager->getCurrentState());
	MariaStateObject* currentObj = mariaStateManager->getCurrentStateObject();

	mariaUI->getCommandBar()->getTextbox()->setUserInput("");
	mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::OK);

	//Jay: Switch case to be refactored later.
	switch(input->getCommandType()) {
	case MariaInputObject::COMMAND_TYPE::INVALID: {
		mariaUI->getCommandBar()->getTextbox()->setQuestionText("I don't understand. Please try again.");
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::UNKNOWN);
		return false;
	}
	break;
	case MariaInputObject::COMMAND_TYPE::EXIT: {
		MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
		mariaStateManager->queueState(MariaStateManager::LOADING, temp);
		temp->setDisplayText("Saving");
		temp->setLoadingDone();
		temp->setQuitAfterLoadingTrue();
		mariaStateManager->transitState();
	}
	break;
	case MariaInputObject::COMMAND_TYPE::ADD_FLOATING: {
		MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, NULL);
		if(toAdd != NULL) {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '" + input->getTitle() + "' has been added!");
			if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
				((MariaUIStateHome*)currentObj)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
			}
		} else {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is a problem adding '" + inputText + "'");
		}
	}
	break;
	case MariaInputObject::COMMAND_TYPE::ADD_DEADLINE: {
		MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, new MariaTime(*input->getEndTime()));
		if(toAdd != NULL) {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '" + input->getTitle() + "' has been added!");
			if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
				((MariaUIStateHome*)currentObj)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
			}
		} else {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is a problem adding '" + inputText + "'");
		}
	}
	break;
	case MariaInputObject::COMMAND_TYPE::ADD_TIMED: {
		MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), new MariaTime(*input->getStartTime()), new MariaTime(*input->getEndTime()));
		if(toAdd != NULL) {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '" + input->getTitle() + "' has been added!");
			if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
				((MariaUIStateHome*)currentObj)->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
			}
		} else {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is a problem adding '" + inputText + "'");
		}
	}
	break;
	case MariaInputObject::COMMAND_TYPE::EDIT_TITLE: {
		string toEditTitle = input->getTitle();

		//Jay: Pending splitting up of conflict and non conflict.
		if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::CONFLICT) {
			int numberToEdit = input->getOptionID();
			MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;
			if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
				//TO DO, transit to edit state.
				MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
				toEditTask->getMariaTask()->setTitle(input->getEditField());
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
					
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Ok, I have updated the title.");
				mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
				mariaStateManager->transitState();					
			}
		} else {
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle);

			if (listOfTasks.size() == 1) {
				if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
					listOfTasks[0]->setTitle(input->getEditField());
					MariaUIStateDisplay* tempObj = (MariaUIStateDisplay*)currentObj;
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Ok, I have updated the title.");
					tempObj->updateUITask();
				}
			} else if (listOfTasks.size() == 0) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

				mariaStateManager->queueState(MariaStateManager::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, mariaTaskManager, toEditTitle));
				mariaStateManager->transitState();
			}
		}
	}
	break;
	case MariaInputObject::COMMAND_TYPE::EDIT_START_TIME: {
		string toEditTitle = input->getTitle();
		
		//Jay: Pending splitting up of conflict and non conflict.
		if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::CONFLICT) {
			int numberToEdit = input->getOptionID();
			MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;
			if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
				MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
				toEditTask->getMariaTask()->setStart(new MariaTime(*input->getEditTime()));
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Consider it done!");
				mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
				mariaStateManager->transitState();
			}
		} else {
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle);

			if (listOfTasks.size() == 1) {
				//Jay: To do, change it to just check if currentObj is a stateDisplay and call updateUI, if not
				//still enable updating but no visual cues.
				if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
					listOfTasks[0]->setStart(new MariaTime(*input->getEditTime()));
					MariaUIStateDisplay* tempObj = (MariaUIStateDisplay*)currentObj;

					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Consider it done!");
					tempObj->updateUITask();
				}
			} else if (listOfTasks.size() == 0) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

				mariaStateManager->queueState(MariaStateManager::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, mariaTaskManager, toEditTitle));
				mariaStateManager->transitState();
			}
		}
	}
	break;
	case MariaInputObject::COMMAND_TYPE::EDIT_END_TIME: {
		string toEditTitle = input->getTitle();

		//Jay: Pending removal of check if its in conflict state.
		if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::CONFLICT) {
			int numberToEdit = input->getOptionID();
			MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;
			if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
				MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
				toEditTask->getMariaTask()->setEnd(new MariaTime(*input->getEditTime()));
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
					
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Consider it done.");
				mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
				mariaStateManager->transitState();
			}
		} else {
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle);

			if (listOfTasks.size() == 1) {
				//Jay: To do, change it to just check if currentObj is a stateDisplay and call updateUI, if not
				//still enable updating but no visual cues.
				if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
					listOfTasks[0]->setEnd(new MariaTime(*input->getEditTime()));
					MariaUIStateDisplay* tempObj = (MariaUIStateDisplay*)currentObj;

					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Consider it done!");
					tempObj->updateUITask();
				}
			} else if (listOfTasks.size() == 0) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

				mariaStateManager->queueState(MariaStateManager::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, mariaTaskManager, toEditTitle));
				mariaStateManager->transitState();
			}
		}
	}
	break;
	case MariaInputObject::COMMAND_TYPE::SHOW_DATE: {
		// Show today, tomorrow, date
		// Date object is obtained through input->getEndTime();
		//Miki data object cannot be reference. Crashes here.
		MariaTime startTime=*(input->getEndTime());
		startTime.setHour(0);
		startTime.setMin(0);
		MariaTime endTime=*(input->getEndTime());
		endTime.setHour(23);
		endTime.setMin(59);
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(&startTime,&endTime);

		mariaUI->getCommandBar()->getTextbox()->setQuestionText("This is what you have on " + MariaTime::convertToDateString(&endTime) + ".");
		mariaStateManager->queueState(MariaStateManager::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, mariaTaskManager, MariaTime::convertToDateString(&startTime), listOfTasks));
		mariaStateManager->transitState();
	}
	break;
	case MariaInputObject::COMMAND_TYPE::SHOW_DATE_RANGE: {
		// Show from start date to end date
		//Miki data object cannot be reference. Crashes here.
		MariaTime startTime=*(input->getStartTime());
		startTime.setHour(0);
		startTime.setMin(0);
		MariaTime endTime=*(input->getEndTime());
		endTime.setHour(23);
		endTime.setMin(59);
		vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(&startTime,&endTime);
			
		mariaUI->getCommandBar()->getTextbox()->setQuestionText("This is what you have from " + MariaTime::convertToDateString(&startTime) + " to " + MariaTime::convertToDateString(&endTime) + ".");
		mariaStateManager->queueState(MariaStateManager::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, mariaTaskManager, MariaTime::convertToDateString(&startTime), listOfTasks));
		mariaStateManager->transitState();
	}
	break;
	case MariaInputObject::COMMAND_TYPE::SHOW_ALL: {
		vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks();

		mariaUI->getCommandBar()->getTextbox()->setQuestionText("Sure, here are all the tasks.");
		mariaStateManager->queueState(MariaStateManager::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, mariaTaskManager, "", listOfTasks));
		mariaStateManager->transitState();
	}
	break;
	case MariaInputObject::COMMAND_TYPE::MARK_DONE: {
	}
	break;
	case MariaInputObject::COMMAND_TYPE::MARK_UNDONE: {
	}
	break;
	case MariaInputObject::COMMAND_TYPE::DELETE_TASK: {
		string toDeleteTitle = input->getTitle();

		if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::CONFLICT) {
			int numberToDelete = input->getOptionID();
			MariaUIStateConflict* tempObj = (MariaUIStateConflict*)currentObj;

			if(numberToDelete > 0 && numberToDelete <= tempObj->getTotalUITask()) {
				MariaUITask* toDeleteTask = tempObj->eraseUITask(numberToDelete-1);
				mariaTaskManager->archiveTask(toDeleteTask->getMariaTask());
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Resolved! Anything else?");
				mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
				mariaStateManager->transitState();
			}
		} else {
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toDeleteTitle);

			if (listOfTasks.size() == 1) {
				if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
					mariaTaskManager->archiveTask(listOfTasks[0]);
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("'" + toDeleteTitle + "' has been deleted!");
					((MariaUIStateHome*)currentObj)->eraseUITask(listOfTasks[0]);
				}
			} else if (listOfTasks.size() == 0) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I remove?");

				mariaStateManager->queueState(MariaStateManager::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, mariaTaskManager, toDeleteTitle));
				mariaStateManager->transitState();
			}
		}
	}
	break;
	case MariaInputObject::COMMAND_TYPE::DELETE_ALL: {
		if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
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
		mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
		mariaStateManager->transitState();
	}
	break;
	case MariaInputObject::COMMAND_TYPE::GO_ADD: {
		// Transit to add screen?
	}
	break;
	case MariaInputObject::COMMAND_TYPE::GO_EDIT: {
		// Transit to edit screen?
	}
	break;
	case MariaInputObject::COMMAND_TYPE::GO_DELETE: {
		// Transit to delete screen?
	}
	break;
	case MariaInputObject::COMMAND_TYPE::GO_SETTINGS: {
	}
	break;
	case MariaInputObject::COMMAND_TYPE::GO_UP: {
	}
	break;
	case MariaInputObject::COMMAND_TYPE::GO_DOWN: {
	}
	break;
	default:
		mariaUI->getCommandBar()->getTextbox()->setQuestionText("It's a valid command, but I'm limited.");
		mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
		mariaStateManager->transitState();
	break;
	}

	delete input;
	input = NULL;

	//todo: call interpreter to generate task & pass to task manager
	return true;
}

void MariaLogic::terminateProgram() {
	quit();
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
