//@author A0111823U
#include "MariaFileManager.h"

const string MariaFileManager::INPUT_FILE = "DEFAULT.SAV";

const string MariaFileManager::TASK_OPEN_FIELD = "[Task]";
const string MariaFileManager::TASK_TITLE_FIELD = "\t[Title]";
const string MariaFileManager::TASK_DESC_FIELD = "\t[Desc]";
const string MariaFileManager::TASK_STARTTIME_FIELD = "\t[Start]";
const string MariaFileManager::TASK_ENDTIME_FIELD = "\t[End]";
const string MariaFileManager::TASK_CREATED_FIELD = "\t[Created]";
const string MariaFileManager::TASK_ISDONE_FIELD = "\t[Done]";
const string MariaFileManager::TASK_CLOSE_FIELD = "[End Task]";

const string MariaFileManager::TIME_FORMAT = "%Y-%m-%d %H:%M:%S";
const string MariaFileManager::TASK_COMPLETED = "Done";
const string MariaFileManager::TASK_NOT_COMPLETED = "Not Done";

MariaFileManager::MariaFileManager(void) {
}

MariaFileManager::~MariaFileManager(void) {
}

// This method is called to open the user input file when the program
// is first run. It parses and stores previously saved user data.
vector<MariaTask*>* MariaFileManager::openFile() {
	ifstream fileReader(INPUT_FILE);
	if (!fileExists()) {
		return createNewFile();
	} else if (fileReader.is_open()) {
		return readFile(&fileReader);
	} else {
		throw exception("File Exists but cannot be opened.");
	}
}

vector<MariaTask*>* MariaFileManager::createNewFile() {
	ofstream fileWriter(INPUT_FILE);
	fileWriter.close();

	return new vector<MariaTask*>();
}

// This method overwrites the user input file 
// with the current data from messageList.
bool MariaFileManager::writeFile(vector<MariaTask*> taskList) {
	ofstream fileWriter(INPUT_FILE);
	if (!fileWriter.is_open()) {
		return false;
	}

	for (MariaTask* task : taskList) {
		fileWriter << taskToString(task);
	}
	fileWriter.close();

	return true;
}


// This method reads and stores data from an input file.
// Requires an already open ifstream to pull data from.
vector<MariaTask*>* MariaFileManager::readFile(ifstream *fileReader) {
	vector<MariaTask*>* taskList = new vector<MariaTask*>();

	while(true) {
		string lineRead;
		string inputText[ATTRIBUTES_PER_TASK] = {""};

		for (int i = 0; i< ATTRIBUTES_PER_TASK; i++) {
			if (getline (*fileReader, lineRead)) {
				if (lineRead == TASK_CLOSE_FIELD) {
					break;
				}
				inputText[i] = lineRead;
			} else {
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

MariaTask* MariaFileManager::stringToTask(string inputText[]) {
	MariaTask* newTask = new MariaTask();
	for (int i = 0; i < ATTRIBUTES_PER_TASK; i++) {
		string field = getFirstWord(inputText[i]);
		string value = removeFirstWord(inputText[i], field);

		if (field == TASK_TITLE_FIELD) {
			newTask->setTitle(value);
		} else if (field == TASK_DESC_FIELD) {
			newTask->setDescription(value);
		} else if (field == TASK_STARTTIME_FIELD) {
			newTask->setStart(stringToTime(value));
		} else if (field == TASK_ENDTIME_FIELD) {
			newTask->setEnd(stringToTime(value));
		} else if (field == TASK_CREATED_FIELD) {
			newTask->setCreated(stringToTime(value));
		} else if (field == TASK_ISDONE_FIELD) {
			newTask->setIsDone(stringToBool(value));
		}
		
	}
	newTask->refreshTaskType();
	return newTask;
}

MariaTime* MariaFileManager::stringToTime(string inputText) {
	if (inputText == "") {
		return NULL;
	} else {
		return new MariaTime(inputText);
	}
}

bool MariaFileManager::stringToBool(string inputText) {
	if (inputText == TASK_COMPLETED) {
		return true;
	} else if (inputText == TASK_NOT_COMPLETED) {
		return false;
	} else {
		throw exception("Corrupted File!");
	}
}

string MariaFileManager::taskToString(MariaTask* task) {
	
	string returnString = TASK_OPEN_FIELD 		 + NEW_LINE + 
							TASK_TITLE_FIELD	 + task->getTitle()					 + NEW_LINE + 
							TASK_DESC_FIELD		 + task->getDescription()			 + NEW_LINE + 
							TASK_STARTTIME_FIELD + timeToString(task->getStart())	 + NEW_LINE + 
							TASK_ENDTIME_FIELD	 + timeToString(task->getEnd())		 + NEW_LINE + 
							TASK_CREATED_FIELD	 + timeToString(task->getCreated())	 + NEW_LINE + 
							TASK_ISDONE_FIELD	 + boolToString(task->getIsDone())	 + NEW_LINE + 
							TASK_CLOSE_FIELD + NEW_LINE;

	return returnString;
}

string MariaFileManager::timeToString(MariaTime* inputTime) {
	if (inputTime == NULL) {
		return ("");
	} else {
		return inputTime->get(TIME_FORMAT);
	}
}

string MariaFileManager::boolToString(bool input) {
	if (input) {
		return TASK_COMPLETED;
	} else {
		return TASK_NOT_COMPLETED;
	}
}

// This method returns the first word from input string
string MariaFileManager::getFirstWord(string inputText) {
	for (int i = 0; i<inputText.length(); i++) {
		if (inputText[i] == ']') {
			return inputText.substr(0, i + 1);
		}
	}
	return inputText;
}

// This method returns the input string without its first word 
string MariaFileManager::removeFirstWord(string inputText, string firstWord) {
	if (inputText.length() <= firstWord.length()) {
		return ("");
	}
	return inputText.substr(firstWord.length());
}