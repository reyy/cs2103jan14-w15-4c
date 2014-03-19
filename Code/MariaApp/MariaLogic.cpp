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
	mariaTaskManager = new MariaTaskManager(mariaFileManager->openFile());
	mariaUI = new MariaUI(this);
	mariaStateManager = new MariaStateManager();

	MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
	mariaStateManager->queueState(MariaStateManager::LOADING,temp);
	temp->setDisplayText("Loading");
	temp->setLoadingDone();
	mariaStateManager->queueState(MariaStateManager::HOME,new MariaUIStateHome((QMainWindow*)mariaUI,mariaTaskManager));
	//mariaUI->setState(MariaUI::INTRO);
	//mariaUI->getLoading()->setDisplayText("Loading");
	//Put loading intensive stuffs in-between changing state to intro and to other state.
	//mariaUI->setState(MariaUI::HOME);

	//Below are things that you can edit.
	//mariaUI->setBackgroundColor("#ff88ff");
	mariaUI->getCommandBar()->getTextbox()->setQuestionText("How can I help you?");
	//mariaUI->getStatus()->setStatus(MariaUIStatus::WAIT);
	mariaUI->getCommandBar()->getTextbox()->setSuggestText("create Meeting tomorrow for discussion");

	
	_beginthread( &MariaLogic::doShowHideWrapper,0, this);
	//CreateThread( NULL,0, (doShowHide), NULL,0,NULL);
}

MariaLogic::~MariaLogic(void) {
	delete mariaFileManager;
	delete mariaTaskManager;
	delete mariaInterpreter;
	delete mariaStateManager;
	delete mariaUI;
}

bool MariaLogic::processCommand(std::string inputText) {
	MariaInputObject* input = mariaInterpreter->parseInput(inputText, mariaStateManager->getCurrentState());
	mariaUI->getCommandBar()->getTextbox()->setUserInput("");

	if(input->getCommandType() == MariaInputObject::CommandType::CommandInvalid) {
		mariaUI->getCommandBar()->getTextbox()->setQuestionText(input->getTitle());
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::UNKNOWN);
		return false;
	} else {
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::OK);

		if(input->getCommandType() == MariaInputObject::CommandType::CommandExit){
			MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
			mariaStateManager->queueState(MariaStateManager::LOADING,temp);
			temp->setDisplayText("Saving");
			temp->setLoadingDone();
			temp->setQuitAfterLoadingTrue();
			mariaStateManager->transitState();
		} else if(input->getCommandType() == MariaInputObject::CommandType::CommandAdd){
			MariaTask *toAdd=mariaTaskManager->addTask(input->getTitle(), input->getStartTime(), input->getEndTime());
			if(toAdd!=NULL){
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '"+ input->getTitle() +"' has been added!");

				//Check if the current state is the home state, do a live add.
				if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->addTask(toAdd);
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
				}
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is problem adding '"+ inputText + "'");
			}
		} else if (input->getCommandType() == MariaInputObject::CommandType::CommandEdit) {
			string toEditTitle = input->getTitle();

			if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::CONFLICT) {
				MariaUIStateConflict* tempObj=(MariaUIStateConflict*)mariaStateManager->getCurrentStateObject();
				//TO DO get total task current DOES NOT return the correct upper bound, it doesn't check if the number is valid.
				//Will crash if the number exceeds the array.
				if(input->getOptionID()>0&&input->getOptionID()<=tempObj->getTotalTask()) {
					//TO DO, transit to edit state.
					mariaStateManager->queueState(MariaStateManager::HOME,new MariaUIStateHome((QMainWindow*)mariaUI,mariaTaskManager));
					mariaStateManager->transitState();
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toEditTitle);

				if (listOfTasks.size() == 1) {
					//Check if the current state is the home state, do a live add.
					if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::HOME) {
						//TO DO, transit to edit state.
						mariaStateManager->queueState(MariaStateManager::HOME,new MariaUIStateHome((QMainWindow*)mariaUI,mariaTaskManager));
						mariaStateManager->transitState();
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are a couple of similar tasks, which one should I edit?");

					mariaStateManager->queueState(MariaStateManager::CONFLICT,new MariaUIStateConflict(toEditTitle,mariaTaskManager,(QMainWindow*)mariaUI));
					mariaStateManager->transitState();
				}
			}
		} else if(input->getCommandType() == MariaInputObject::CommandType::CommandShowAll){
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Sure, here's the calendar.");
			mariaStateManager->queueState(MariaStateManager::SHOW,new MariaUIStateShow((QMainWindow*)mariaUI,mariaTaskManager,MariaUIStateShow::VIEW_TYPE::YEAR,MariaTime::getCurrentTime()));
			mariaStateManager->transitState();
		} else if (input->getCommandType() == MariaInputObject::CommandType::CommandDelete) {
			string toDeleteTitle = input->getTitle();

			if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::CONFLICT) {
				MariaUIStateConflict* tempObj=(MariaUIStateConflict*)mariaStateManager->getCurrentStateObject();
				//TO DO get total task current DOES NOT return the correct upper bound, it doesn't check if the number is valid.
				//Will crash if the number exceeds the array.
				if(input->getOptionID()>0&&input->getOptionID()<=tempObj->getTotalTask()) {
					MariaUITask* toDeleteTask = tempObj->eraseTask(input->getOptionID()-1);
					mariaTaskManager->archiveTask(toDeleteTask->getMariaTask());
					mariaFileManager->writeFile(mariaTaskManager->getAllTasks());

					mariaUI->getCommandBar()->getTextbox()->setQuestionText("Resolved! Anything else?");
					mariaStateManager->queueState(MariaStateManager::HOME,new MariaUIStateHome((QMainWindow*)mariaUI,mariaTaskManager));
					mariaStateManager->transitState();
				}
			} else {
				vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(toDeleteTitle);

				if (listOfTasks.size() == 1) {
					//Check if the current state is the home state, do a live add.
					if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::HOME) {
						mariaUI->getCommandBar()->getTextbox()->setQuestionText("'"+listOfTasks[0]->getTitle()+"' has been deleted!");
						((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->eraseTask(listOfTasks[0]);
						mariaTaskManager->archiveTask(listOfTasks[0]);
						mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
					}
				} else if (listOfTasks.size() == 0) {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("I couldn't find anything related. Try again.");
				} else {
					mariaUI->getCommandBar()->getTextbox()->setQuestionText("There are a couple of similar tasks, which one should I remove?");

					mariaStateManager->queueState(MariaStateManager::CONFLICT,new MariaUIStateConflict(toDeleteTitle,mariaTaskManager,(QMainWindow*)mariaUI));
					mariaStateManager->transitState();
				}
			}
		} else if (input->getCommandType() == MariaInputObject::CommandType::CommandDeleteAll) {
			//Check if the current state is the home state, do a live add.
			if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::HOME) {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("All tasks have been deleted.");
				((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->eraseAllTask();

				// TODO: replace this part to delete all
				vector<MariaTask*> listOfTasks = mariaTaskManager->getAllTasks();
				for (int i = 0; i < listOfTasks.size(); i++) {
					mariaTaskManager->archiveTask(listOfTasks[i]);
				}
				mariaFileManager->writeFile(mariaTaskManager->getAllTasks());
			}
		} else if (input->getCommandType() == MariaInputObject::CommandType::CommandGoToHome) {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("How can I help you?");
			mariaStateManager->queueState(MariaStateManager::HOME,new MariaUIStateHome((QMainWindow*)mariaUI,mariaTaskManager));
			mariaStateManager->transitState();
		} else if (input->getCommandType() == MariaInputObject::CommandType::CommandGoUp) {
			// Here be your Up command.
		} else if (input->getCommandType() == MariaInputObject::CommandType::CommandGoDown) {
			// Here be your Down command. Carry on 20.
		} else {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("It's a valid command, but I'm limited.");
			mariaStateManager->queueState(MariaStateManager::HOME,new MariaUIStateHome((QMainWindow*)mariaUI,mariaTaskManager));
			mariaStateManager->transitState();
		}
	}

	//todo: call interpreter to generate task & pass to task manager
	return true;
}

void MariaLogic::terminateProgram() {
	quit();
}

void MariaLogic::doShowHide(){
	RegisterHotKey(NULL,1,MOD_CONTROL | MOD_NOREPEAT,VK_SPACE);
	MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_HOTKEY){
 			emit mariaUI->triggerShowHideEvent();
		}
    }
}

void __cdecl MariaLogic::doShowHideWrapper(void* mariaLogic){
	
	static_cast<MariaLogic*>(mariaLogic)->doShowHide();
}

int main(int argc, char *argv[]) {
	MariaLogic *mariaLogic = new MariaLogic(argc, argv);

	int returnCode = QApplication::exec();
	delete mariaLogic;

	return returnCode;
}
