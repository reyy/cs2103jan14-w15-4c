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
		}
	}
	mariaUI = new MariaUI(this);
	mariaStateManager = new MariaStateManager();

	MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
	mariaStateManager->queueState(MariaStateManager::LOADING, temp);
	temp->setDisplayText("Loading");
	temp->setLoadingDone();
	mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
	//mariaUI->setState(MariaUI::INTRO);
	//mariaUI->getLoading()->setDisplayText("Loading");
	//Put loading intensive stuffs in-between changing state to intro and to other state.
	//mariaUI->setState(MariaUI::HOME);

	//Below are things that you can edit.
	//mariaUI->setBackgroundColor("#ff88ff");
	mariaUI->getCommandBar()->getTextbox()->setQuestionText("How can I help you?");
	//mariaUI->getStatus()->setStatus(MariaUIStatus::WAIT);
	mariaUI->getCommandBar()->getTextbox()->setSuggestText("create Meeting tomorrow for discussion");

	
	_beginthread( &MariaLogic::doShowHideWrapper, 0, this);
	//CreateThread( NULL, 0, (doShowHide), NULL, 0, NULL);
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
		return true;
	}
	return false;
}

bool MariaLogic::processCommand(std::string inputText) {
	MariaInputObject* input = mariaInterpreter->parseInput(inputText, mariaStateManager->getCurrentState());
	mariaUI->getCommandBar()->getTextbox()->setUserInput("");

	if(input->getCommandType() == MariaInputObject::COMMAND_TYPE::INVALID) {
		mariaUI->getCommandBar()->getTextbox()->setQuestionText("I don't understand. Please try again.");
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::UNKNOWN);
		return false;
	} else {
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::OK);

		if(input->getCommandType() == MariaInputObject::COMMAND_TYPE::EXIT) {
			MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
			mariaStateManager->queueState(MariaStateManager::LOADING, temp);
			temp->setDisplayText("Saving");
			temp->setLoadingDone();
			temp->setQuitAfterLoadingTrue();
			mariaStateManager->transitState();
		} else if(input->getCommandType() == MariaInputObject::COMMAND_TYPE::ADD_FLOATING) {
			MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, NULL);
			if(toAdd != NULL) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '" + input->getTitle() + "' has been added!");

				//Check if the current state is the home state, do a live add.
				if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
				}
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is a problem adding '" + inputText + "'");
			}
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::ADD_DEADLINE) {
			MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), NULL, new MariaTime(*input->getEndTime()));
			if(toAdd != NULL) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '" + input->getTitle() + "' has been added!");

				//Check if the current state is the home state, do a live add.
				if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
				}
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is a problem adding '" + inputText + "'");
			}
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::ADD_TIMED) {
			MariaTask *toAdd = mariaTaskManager->addTask(input->getTitle(), new MariaTime(*input->getStartTime()), new MariaTime(*input->getEndTime()));
			if(toAdd != NULL) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '" + input->getTitle() + "' has been added!");

				//Check if the current state is the home state, do a live add.
				if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->addUITask(toAdd, MariaUITask::DISPLAY_TYPE::NORMAL);
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
				}
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is a problem adding '" + inputText + "'");
			}
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::EDIT_TITLE) {
			string toEditTitle = input->getTitle();

			if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::CONFLICT) {
				int numberToEdit = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)mariaStateManager->getCurrentStateObject();
				//TO DO get total task current DOES NOT return the correct upper bound, it doesn't check if the number is valid.
				//Will crash if the number exceeds the array.
				if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
					//TO DO, transit to edit state.
					MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
					toEditTask->getMariaTask()->setTitle(input->getEditField());
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

					mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
					mariaStateManager->transitState();
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Edit: DONE!");
					mariaStateManager->runActiveUpdate();
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle);

				if (listOfTasks.size() == 1) {
					//Check if the current state is the home state, do a live add.
					if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
						//TO DO, transit to edit state.
						listOfTasks[0]->setTitle(input->getEditField());
						mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
						mariaStateManager->transitState();
						mariaStateManager->runActiveUpdate();
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

					mariaStateManager->queueState(MariaStateManager::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, mariaTaskManager, toEditTitle));
					mariaStateManager->transitState();
				}
			}
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::EDIT_START_TIME) {
			string toEditTitle = input->getTitle();

			if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::CONFLICT) {
				int numberToEdit = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)mariaStateManager->getCurrentStateObject();
				//TO DO get total task current DOES NOT return the correct upper bound, it doesn't check if the number is valid.
				//Will crash if the number exceeds the array.
				if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
					//TO DO, transit to edit state.
					MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
					toEditTask->getMariaTask()->setStart(new MariaTime(*input->getEditTime()));
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
					mariaStateManager->runActiveUpdate();

					mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
					mariaStateManager->transitState();
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Edit: DONE!");
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle);

				if (listOfTasks.size() == 1) {
					//Check if the current state is the home state, do a live add.
					if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
						//TO DO, transit to edit state.
						listOfTasks[0]->setStart(new MariaTime(*input->getEditTime()));
						mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
						mariaStateManager->transitState();
						mariaStateManager->runActiveUpdate();
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

					mariaStateManager->queueState(MariaStateManager::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, mariaTaskManager, toEditTitle));
					mariaStateManager->transitState();
				}
			}
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::EDIT_END_TIME) {
			string toEditTitle = input->getTitle();

			if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::CONFLICT) {
				int numberToEdit = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)mariaStateManager->getCurrentStateObject();
				//TO DO get total task current DOES NOT return the correct upper bound, it doesn't check if the number is valid.
				//Will crash if the number exceeds the array.
				if(numberToEdit > 0 && numberToEdit <= tempObj->getTotalUITask()) {
					//TO DO, transit to edit state.
					MariaUITask* toEditTask = tempObj->eraseUITask(numberToEdit-1);
					toEditTask->getMariaTask()->setEnd(new MariaTime(*input->getEditTime()));
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
					mariaStateManager->runActiveUpdate();

					mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
					mariaStateManager->transitState();
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Edit: DONE!");
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle);

				if (listOfTasks.size() == 1) {
					//Check if the current state is the home state, do a live add.
					if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
						//TO DO, transit to edit state.
						listOfTasks[0]->setEnd(new MariaTime(*input->getEditTime()));
						mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
						mariaStateManager->transitState();
						mariaStateManager->runActiveUpdate();
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I edit?");

					mariaStateManager->queueState(MariaStateManager::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, mariaTaskManager, toEditTitle));
					mariaStateManager->transitState();
				}
			}
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::SHOW_DATE) {
			// Show today, tomorrow, date
			// Date object is obtained through input->getEndTime();
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::SHOW_DATE_RANGE) {
			// Show from start date to end date
		} else if(input->getCommandType() == MariaInputObject::COMMAND_TYPE::SHOW_ALL) {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Sure, here's the calendar.");
			mariaStateManager->queueState(MariaStateManager::SHOW, new MariaUIStateShow((QMainWindow*)mariaUI, mariaTaskManager, MariaUIStateShow::VIEW_TYPE::YEAR, MariaTime::getCurrentTime()));
			mariaStateManager->transitState();
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::MARK_DONE) {
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::MARK_UNDONE) {
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::DELETE_TASK) {
			string toDeleteTitle = input->getTitle();

			if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::CONFLICT) {
				int numberToDelete = input->getOptionID();
				MariaUIStateConflict* tempObj = (MariaUIStateConflict*)mariaStateManager->getCurrentStateObject();
				//TO DO get total task current DOES NOT return the correct upper bound, it doesn't check if the number is valid.
				//Will crash if the number exceeds the array.
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
					//Check if the current state is the home state, do a live add.
					if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
						mariaTaskManager->archiveTask(listOfTasks[0]);
						mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
						mariaUI->getCommandBar()->getTextbox()->setQuestionText("'" + toDeleteTitle + "' has been deleted!");
						((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->eraseUITask(listOfTasks[0]);
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are similar tasks, which one should I remove?");

					mariaStateManager->queueState(MariaStateManager::CONFLICT, new MariaUIStateConflict((QMainWindow*)mariaUI, mariaTaskManager, toDeleteTitle));
					mariaStateManager->transitState();
				}
			}
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::DELETE_ALL) {
			//Check if the current state is the home state, do a live add.
			if(mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
				// TODO: replace this part to delete all
				vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks();
				for (int i = 0; i < listOfTasks.size(); i++ ) {
					mariaTaskManager->archiveTask(listOfTasks[i]);
				}
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

				mariaUI->getCommandBar()->getTextbox()->setQuestionText("All tasks have been deleted.");
				((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->eraseAllUITask();
			}
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::UNDO) {
			if(processUndo()) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Undo was sucessful");
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Nothing to Undo.");
			}
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::GO_HOME) {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("How can I help you?");
			mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
			mariaStateManager->transitState();
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::GO_ADD) {
			// Transit to add screen?
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::GO_EDIT) {
			// Transit to edit screen?
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::GO_DELETE) {
			// Transit to delete screen?
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::GO_SETTINGS) {
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::GO_UP) {
		} else if (input->getCommandType() == MariaInputObject::COMMAND_TYPE::GO_DOWN) {
		} else {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("It's a valid command, but I'm limited.");
			mariaStateManager->queueState(MariaStateManager::HOME, new MariaUIStateHome((QMainWindow*)mariaUI, mariaTaskManager));
			mariaStateManager->transitState();
		}
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
