#include "MariaLogic.h"
#include "MariaUIStatus.h"

MariaLogic::MariaLogic(int argc, char *argv[]) : QApplication(argc, argv) {
	mariaInterpreter = new MariaInterpreter();
	mariaFileManager = new MariaFileManager();
	mariaTaskManager = new MariaTaskManager(mariaFileManager->openFile());
	mariaUI = new MariaUI(this,mariaTaskManager);

	mariaUI->setState(MariaUI::INTRO);
	mariaUI->getLoading()->setDisplayText("Loading");

	//Put loading intensive stuffs in-between changing state to intro and to other state.
	try{
		//mariaTaskManager = new MariaTaskManager(/*mariaFileManager->openFile()*/);
	}catch(exception& e){
		//todo: prompt user, file cannot be written!
		//mariaTaskManager = new MariaTaskManager();
	}
	
	mariaUI->setState(MariaUI::HOME);


	//Below are things that you can edit.
	//mariaUI->setBackgroundColor("#ff88ff");
	mariaUI->getTextbox()->setQuestionText("What would you like to do?");
	mariaUI->getStatus()->setStatus(MariaUIStatus::WAIT);
	mariaUI->getTextbox()->setSuggestText("create Meeting tomorrow for discussion");
}

MariaLogic::~MariaLogic(void) {
	delete mariaFileManager;
	delete mariaTaskManager;
	delete mariaInterpreter;
	delete mariaUI;
}

bool MariaLogic::processCommand(std::string inputText) {
	MariaInterpreter::CommandType commandType = mariaInterpreter->getCommandType(inputText);
	mariaUI->getTextbox()->setUserInput("");

	if(commandType == MariaInterpreter::CommandType::Invalid) {
		mariaUI->getTextbox()->setQuestionText("I don't understand. Please try again.");
		mariaUI->getStatus()->setStatus(MariaUIStatus::UNKNOWN);
		return false;
	} else {
		mariaUI->getStatus()->setStatus(MariaUIStatus::OK);

		if(commandType == MariaInterpreter::CommandType::Exit){
			mariaUI->setState(MariaUI::INTRO);
			mariaUI->getLoading()->setDisplayText("Saving");
			//Do uploading to google etc.
			//Save files operation etc.
			mariaUI->setState(MariaUI::QUIT);
		} else if(commandType == MariaInterpreter::CommandType::Quit){
			quit();
		} else if(commandType == MariaInterpreter::CommandType::AddFloatingTask){
			std::string taskTitle = mariaInterpreter->getTitle(inputText);
			if(mariaTaskManager->addTask(taskTitle, NULL, NULL)){
				mariaUI->getTextbox()->setQuestionText("Task '"+ taskTitle +"' has been added!");
				mariaFileManager->writeFile(mariaTaskManager->findTask(""));
			} else {
				mariaUI->getTextbox()->setQuestionText("There is problem adding '"+ inputText + "'");
			}
		} else if(commandType == MariaInterpreter::CommandType::ShowAllTask){
			mariaUI->getTextbox()->setQuestionText("Sure, here's a calendar for demo purposes.");
			mariaUI->setState(MariaUI::CALENDAR);
		} else {
			mariaUI->getTextbox()->setQuestionText("Its a valid command, but I'm limited.");
			mariaUI->setState(MariaUI::HOME);
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