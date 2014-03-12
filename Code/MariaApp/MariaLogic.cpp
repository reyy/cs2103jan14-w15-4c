#include "MariaLogic.h"
#include "MariaUIStatus.h"
#include "MariaUIStateLoading.h"
#include "MariaUIStateHome.h"
#include "MariaUIStateShow.h"

MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
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
	//MariaInterpreter::CommandType commandType = mariaInterpreter->getCommandType(inputText);
	MariaInputObject* input = mariaInterpreter->parseInput(inputText);
	mariaUI->getCommandBar()->getTextbox()->setUserInput("");

	if(input->getCommandType() == MariaInputObject::CommandInvalid) {
		mariaUI->getCommandBar()->getTextbox()->setQuestionText("I don't understand. Please try again.");
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::UNKNOWN);
		return false;
	} else {
		mariaUI->getCommandBar()->getStatus()->setStatus(MariaUIStatus::OK);

		if(input->getCommandType() == MariaInputObject::CommandExit){
			MariaUIStateLoading *temp = new MariaUIStateLoading((QMainWindow*)mariaUI);
			mariaStateManager->queueState(MariaStateManager::LOADING,temp);
			temp->setDisplayText("Saving");
			temp->setLoadingDone();
			temp->setQuitAfterLoadingTrue();
			mariaStateManager->transitState();
		} else if(input->getCommandType() == MariaInputObject::CommandAdd){
			MariaTask *toAdd=mariaTaskManager->addTask(input->getTitle(), input->getStartTime(), input->getEndTime());

			if(toAdd!=NULL){
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("Task '"+ input->getTitle() +"' has been added!");
				
				//Check if the current state is the home state, do a live add.
				if(mariaStateManager->getCurrentState()==MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->addTask(toAdd);
					mariaFileManager->writeFile(mariaTaskManager->findTask(""));
				}
			} else {
				mariaUI->getCommandBar()->getTextbox()->setQuestionText("There is problem adding '"+ inputText + "'");
			}
		} else if (input->getCommandType() == MariaInputObject::CommandEdit) {
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle());

			if (listOfTasks.size() == 1) {
				switch (input->getEditType()) {
				case MariaInputObject::EditTitle:
					listOfTasks[0]->setTitle(input->getEditField());
					break;
				case MariaInputObject::EditStartTime:
					break;
				case MariaInputObject::EditEndTime:
					break;
				default:
					break;
				}
				if (mariaStateManager->getCurrentState() == MariaStateManager::STATE_TYPE::HOME) {
					((MariaUIStateHome*)mariaStateManager->getCurrentStateObject())->updateTask();
				}
			} else if (listOfTasks.size() == 0) {
				// Task not found
			} else {
				// Conflict! More than 1 task found.
			}
		} else if(input->getCommandType() == MariaInputObject::CommandType::CommandShowAll){
			mariaUI->getCommandBar()->getTextbox()->setQuestionText("Sure, here's a calendar for demo purposes.");
			mariaStateManager->queueState(MariaStateManager::SHOW,new MariaUIStateShow((QMainWindow*)mariaUI));
			mariaStateManager->transitState();
		} else if (input->getCommandType() == MariaInputObject::CommandType::CommandDelete) {
			vector<MariaTask*> listOfTasks = mariaTaskManager->findTask(input->getTitle());

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

int main(int argc, char *argv[]) {
	MariaLogic *mariaLogic = new MariaLogic(argc, argv);

	int returnCode = QApplication::exec();
	delete mariaLogic;

	return returnCode;
}