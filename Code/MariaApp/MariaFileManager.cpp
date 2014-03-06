#include "MariaFileManager.h"

const string MariaFileManager::INPUT_FILE = "DEFAULT.SAV";
const string MariaFileManager::TASK_OPEN_FIELD = "[Task]";
const string MariaFileManager::TASK_TITLE_FIELD = "\t[Title]";
const string MariaFileManager::TASK_DESC_FIELD = "\t[Desc]";
const string MariaFileManager::TASK_STARTTIME_FIELD = "\t[Start]";
const string MariaFileManager::TASK_ENDTIME_FIELD = "\t[End]";
const string MariaFileManager::TASK_CLOSE_FIELD = "[End Task]";

MariaFileManager::MariaFileManager(void){

}


MariaFileManager::~MariaFileManager(void){
}

// This method is called to open the user input file when the program
// is first run. It parses and stores previously saved user data.
bool MariaFileManager::openFile(){
	ifstream fileReader(INPUT_FILE);
	if (fileReader.is_open()){
		readFile(&fileReader);
		fileReader.close();
	}else if (fileExists()) {
		return false;
	}

	return true;
}


// This method overwrites the user input file  
// with the current data from messageList.
bool MariaFileManager::writeFile(){
	ofstream fileWriter(INPUT_FILE);
	if(!fileWriter.is_open()){
		return false;
	}

	/*for(string message : messageList){
		fileWriter << message << endl;
	}*/
	fileWriter.close();

	return true;
}


// This method reads and stores data from an input file.
// Requires an already open ifstream to pull data from.
void MariaFileManager::readFile(ifstream *fileReader){
	string lineRead;
	while(getline (*fileReader,lineRead)){
		//this->messageList.push_back(lineRead);
	}
}

bool MariaFileManager::fileExists() {
    struct stat fileInfo;
	return stat(INPUT_FILE.c_str(), &fileInfo) == 0;
}

MariaTask* MariaFileManager::stringToTask(string inputText[]){
	MariaTask* newTask = new MariaTask();
	for(int i = 0; i < ATTRIBUTES_PER_TASK; i++){
		string field = getFirstWord(inputText[i]);
		string value = removeFirstWord(inputText[i], field);

		if(field == TASK_TITLE_FIELD){
			newTask->setTitle(value);
		}else if (field == TASK_DESC_FIELD){
			newTask->setDescription(value);
		}else if (field == TASK_STARTTIME_FIELD){
			newTask->setStart(new MariaTime());
		}else if (field == TASK_ENDTIME_FIELD){
			newTask->setEnd(new MariaTime());
		}
		
	}
	return newTask;
}

// This method returns the first word from input string
string MariaFileManager::getFirstWord(string inputText){
	for(int i=0; i<inputText.length(); i++){
		if(inputText[i] == ' '){
			return inputText.substr(0,i);
		}
	}
	return inputText;
}

// This method returns the input string without its first word 
string MariaFileManager::removeFirstWord(string inputText, string firstWord){
	if(inputText.length() <= firstWord.length()){
		return "";
	}
	return inputText.substr(firstWord.length());
}