#include "MariaLogic.h"
#include "MariaUIStatus.h"
#include "MariaUIStateLoading.h"
#include "MariaUIStateHome.h"

MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	mariaInterpreter = new MariaInterpreter();
	mariaTaskManager = new MariaTaskManager();
	mariaFileManager = new MariaFileManager();
	mariaUI = new MariaUI(this);
	mariaStateManager = new MariaStateManager();
	
	MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
	mariaStateManager->queueState(MariaStateManager::LOADING,temp);
	temp->setDisplayText("Loading");
	temp->setLoadingDone();
	mariaStateManager->queueState(MariaStateManager::HOME,new MariaUIStateHome((QMainWindow*)mariaUI));
	//mariaUI->setState(MariaUI::INTRO);
	//mariaUI->getLoading()->setDisplayText("Loading");
	//Put loading intensive stuffs in-between changing state to intro and to other state.
	//mariaUI->setState(MariaUI::HOME);


	//Below are things that you can edit.
	//mariaUI->setBackgroundColor("#ff88ff");
	mariaUI->getCommandBar()->getTextbox()->setQuestionText("What would you like to do?");
	//mariaUI->getStatus()->setStatus(MariaUIStatus::WAIT);
	mariaUI->getCommandBar()->getTextbox()->setSuggestText("create Meeting tomorrow for discussion");
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
		} else if(commandType == MariaInterpreter::CommandType::Quit){
			quit();
		} else if(commandType == MariaInterpreter::CommandType::AddFloatingTask){
			std::string taskTitle = mariaInterpreter->getTitle(inputText);
			if(mariaTaskManager->addTask(taskTitle, NULL, NULL)){
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '"+ taskTitle +"' has been added!");
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is problem adding '"+ inputText + "'");
			}
		} else if(commandType == MariaInterpreter::CommandType::ShowAllTask){
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Sure, here's a calendar for demo purposes.");
			//mariaStateManager->queueState(MariaStateManager::CALENDAR);
			//mariaStateManager->transitState();
		} else {
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Its a valid command, but I'm limited.");
			//mariaStateManager->queueState(MariaStateManager::HOME);
			//mariaStateManager->transitState();
		}
	}

	//todo: call interpreter to generate task & pass to task manager
	return true;
}

int main(int argc, char *argv[]) {
	MariaLogic *mariaLogic = new MariaLogic(argc, argv);

	int returnCode = QApplication::exec();
	delete mariaLogic;

	return returnCode;
}