#include "MariaLogic.h"
#include "MariaUIStatus.h"
#include "MariaUIStateLoading.h"
#include "MariaUIStateHome.h"
#include "MariaUIStateShow.h"


MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	QApplication::setWindowIcon(QIcon(QString::fromStdString("Resources/maria_icon.png")));

	mariaInterpreter = new MariaInterpreter();
	mariaFileManager = new MariaFileManager();
 	mariaTaskManager = new MariaTaskManager(mariaFileManager->openFile());
	mariaUI = new MariaUI(this);
	mariaStateManager = new MariaStateManager();
	
	MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
	mariaStateManager->queueState(MariaStateManager::LOADING,temp);
	temp->setDisplayText("Loading");
	temp->setLoadingDone();
	mariaStateManager->queueState(MariaStateManager::HOME,new MariaUIStateHome(mariaTaskManager,(QMainWindow*)mariaUI));
	//mariaUI->setState(MariaUI::INTRO);
	//mariaUI->getLoading()->setDisplayText("Loading");
	//Put loading intensive stuffs in-between changing state to intro and to other state.
	//mariaUI->setState(MariaUI::HOME);
	
	//QCoreApplication::installNativeEventFilter(new MyXcbEventFilter());
	//Below are things that you can edit.
	//mariaUI->setBackgroundColor("#ff88ff");
	mariaUI->getCommandBar()->getTextbox()->setQuestionText("What would you like to do?");
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
	MariaInterpreter::CommandType commandType = mariaInterpreter->getCommandType(inputText);
	mariaUI->getCommandBar()->getTextbox()->setUserInput("");

	if(commandType == MariaInterpreter::CommandType::Invalid) {
		mariaUI->getCommandBar()->getTextbox()->setQuestionText("I don't understand. Please try again.");
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::UNKNOWN);
		return false;
	} else {
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::OK);

		if(commandType == MariaInterpreter::CommandType::Exit){
			MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
			mariaStateManager->queueState(MariaStateManager::LOADING,temp);
			temp->setDisplayText("Saving");
			temp->setLoadingDone();
			temp->setQuitAfterLoadingTrue();
			mariaStateManager->transitState();
		} else if(commandType == MariaInterpreter::CommandType::AddFloatingTask){
			std::string taskTitle = mariaInterpreter->getTitle(inputText);

			MariaTask *toAdd=mariaTaskManager->addTask(taskTitle, NULL, NULL);
			if(toAdd!=NULL){
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '"+ taskTitle +"' has been added!");
				
				//Check if the current state is the home state, do a live add.
				if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->addTask(toAdd);
					mariaFileManager->writeFile(mariaTaskManager->findTask(""));
				}
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is problem adding '"+ inputText + "'");
			}
		} else if (commandType == MariaInterpreter::CommandType::AddDeadlineTask) {
			std::string taskTitle = mariaInterpreter->getTitle(inputText);
			MariaTime* endTime = mariaInterpreter->getEndTime(inputText);

			MariaTask *toAdd=mariaTaskManager->addTask(taskTitle, NULL, endTime);
			if(toAdd!=NULL){
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '"+ taskTitle +"' has been added!");
				
				//Check if the current state is the home state, do a live add.
				if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->addTask(toAdd);
					mariaFileManager->writeFile(mariaTaskManager->findTask(""));
				}
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is problem adding '"+ inputText + "'");
			}
		} else if (commandType == MariaInterpreter::CommandType::AddTimedTask) {
			std::string taskTitle = mariaInterpreter->getTitle(inputText);
			MariaTime* startTime = mariaInterpreter->getStartTime(inputText);
			MariaTime* endTime = mariaInterpreter->getEndTime(inputText);

			MariaTask *toAdd=mariaTaskManager->addTask(taskTitle, startTime, endTime);
			if(toAdd!=NULL){
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '"+ taskTitle +"' has been added!");
				
				//Check if the current state is the home state, do a live add.
				if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->addTask(toAdd);
					mariaFileManager->writeFile(mariaTaskManager->findTask(""));
				}
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is problem adding '"+ inputText + "'");
			}
		} else if (commandType == MariaInterpreter::CommandType::EditTask) {
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(mariaInterpreter->getTitle(inputText));

			if (listOfTasks.size() == 1) {
				listOfTasks[0]->setTitle(mariaInterpreter->getNewTitle(inputText));
				if (mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->updateTask();
				}
			} else if (listOfTasks.size() == 0) {
				// Task not found
			} else {
				// Conflict! More than 1 task found.
			}
		} else if(commandType == MariaInterpreter::CommandType::ShowAllTask){
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Sure, here's a calendar for demo purposes.");
			mariaStateManager->queueState(MariaStateManager::SHOW,new MariaUIStateShow((QMainWindow*)mariaUI));
			mariaStateManager->transitState();
		} else if (commandType == MariaInterpreter::CommandType::DeleteTask) {
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(mariaInterpreter->getTitle(inputText));

			if (listOfTasks.size() == 1) {
				//Check if the current state is the home state, do a live add.
				if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->eraseTask(listOfTasks[0]);
					mariaTaskManager->archiveTask(listOfTasks[0]);
					mariaFileManager->writeFile(mariaTaskManager->findTask(""));
				}
			} else if (listOfTasks.size() == 0) {
				// Task not found.
			} else {
				// More than 1 task
			}
		} else {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Its a valid command, but I'm limited.");
			//mariaStateManager->queueState(MariaStateManager::HOME);
			//mariaStateManager->transitState();
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
