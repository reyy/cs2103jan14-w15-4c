#include "MariaFileManager.h"

const string MariaFileManager::INPUT_FILE = "DEFAULT.SAV";
const string MariaFileManager::TASK_OPEN_FIELD = "[Task]";
const string MariaFileManager::TASK_TITLE_FIELD = "\t[Title]";
const string MariaFileManager::TASK_DESC_FIELD = "\t[Desc]";
const string MariaFileManager::TASK_STARTTIME_FIELD = "\t[Start]";
const string MariaFileManager::TASK_ENDTIME_FIELD = "\t[End]";
const string MariaFileManager::TASK_CLOSE_FIELD = "[End Task]";
const string MariaFileManager::TIME_FORMAT = "%Y-%m-%d %H:%M:%S";

MariaFileManager::MariaFileManager(void){
	
}

MariaFileManager::~MariaFileManager(void){
}

// This method is called to open the user input file when the program
// is first run. It parses and stores previously saved user data.
vector<MariaTask*>* MariaFileManager::openFile(){
	ifstream fileReader(INPUT_FILE);
	if (!fileExists()){
		return createNewFile();
	}else if (fileReader.is_open()){
		return readFile(&fileReader);
	}else{
		throw exception("File Exists but cannot be opened.");
	}
}

vector<MariaTask*>* MariaFileManager::createNewFile(){
	ofstream fileWriter(INPUT_FILE);
	fileWriter.close();

	return new vector<MariaTask*>();
}

// This method overwrites the user input file  
// with the current data from messageList.
bool MariaFileManager::writeFile(vector<MariaTask*> taskList){
	ofstream fileWriter(INPUT_FILE);
	if(!fileWriter.is_open()){
		return false;
	}

	for(MariaTask* task : taskList){
		fileWriter << taskToString(task);
	}
	fileWriter.close();

	return true;
}


// This method reads and stores data from an input file.
// Requires an already open ifstream to pull data from.
vector<MariaTask*>* MariaFileManager::readFile(ifstream *fileReader){
	vector<MariaTask*>* taskList = new vector<MariaTask*>();

	while(true){
		string lineRead;
		string inputText[ATTRIBUTES_PER_TASK];

		for(int i = 0; i< ATTRIBUTES_PER_TASK; i++){
			if(getline (*fileReader,lineRead)){
				inputText[i] = lineRead;
			}else{
				fileReader->close();
				return taskList;
			}
		}
		taskList->push_back(stringToTask(inputText));
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
			newTask->setStart(stringToTime(value));
		}else if (field == TASK_ENDTIME_FIELD){
			newTask->setEnd(stringToTime(value));
		}
		
	}
	return newTask;
}

MariaTime* MariaFileManager::stringToTime(string inputText){
	if(inputText == ""){
		return NULL;
	}else{
		return new MariaTime(inputText,TIME_FORMAT);
	}
}

string MariaFileManager::taskToString(MariaTask *task){
	
	string returnString = TASK_OPEN_FIELD 		 + NEW_LINE +
							TASK_TITLE_FIELD	 + task->getTitle()			+ NEW_LINE +
							TASK_DESC_FIELD		 + task->getDescription()	+ NEW_LINE +
							TASK_STARTTIME_FIELD + task->getStart()->get(TIME_FORMAT)	+ NEW_LINE +
							TASK_ENDTIME_FIELD	 + task->getEnd()->get(TIME_FORMAT)	+ NEW_LINE +
							TASK_CLOSE_FIELD    + NEW_LINE;

	return returnString;
}

// This method returns the first word from input string
string MariaFileManager::getFirstWord(string inputText){
	for(int i=0; i<inputText.length(); i++){
		if(inputText[i] == ']'){
			return inputText.substr(0,i+1);
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