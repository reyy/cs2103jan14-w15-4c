//@author A0111823U
#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(IntegratedTest) {
public:
	//Integrated Test Case that simulates adding and removing a task
	TEST_METHOD(MSSTest_AddRemoveTask) {
		string inputString = "create UniqueTask101";

		MariaInterpreter* interpreter = new MariaInterpreter();
		MariaFileManager* fileManager = new MariaFileManager();
		MariaTaskManager* taskManager = new MariaTaskManager(fileManager->openFile());

		MariaInputObject* input = interpreter->parseInput(inputString);

		if (input->getCommandType() == MariaInputObject::CommandType::ADD_FLOATING) {
			MariaTask* toAdd = taskManager->addTask(input->getTitle(), NULL, NULL);
			if (toAdd != NULL) {
				fileManager->writeFile(taskManager->getAllTasks());
			}

			Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 1);


			taskManager->archiveTask(taskManager->findTask("UniqueTask101")[0]);
			fileManager->writeFile(taskManager->getAllTasks());

			Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 0);

			delete fileManager;
			delete taskManager;

			//Reload the managers to check if task is still there
			interpreter = new MariaInterpreter();
			fileManager = new MariaFileManager();
			taskManager = new MariaTaskManager(fileManager->openFile());

			Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 0);

			delete fileManager;
			delete taskManager;

		} else {
			Assert::IsTrue(false);
		}
	}

	//Integrated Test Case that simulates adding and removing a task
	TEST_METHOD(MSSTest_AddEditRemoveTask) {
		string inputString[4] = {"create UniqueTask101", 
								"edit UniqueTask101 change title UnqiueTask202", 
								"delete UnqiueTask101", 
								"delete UnqiueTask202"};

		MariaInterpreter* interpreter = new MariaInterpreter();
		MariaFileManager* fileManager = new MariaFileManager();
		MariaTaskManager* taskManager = new MariaTaskManager(fileManager->openFile());

		//Test for 'create UniqueTask101'
		MariaInputObject* input = interpreter->parseInput(inputString[0]);

		Assert::IsTrue(input->getCommandType() == MariaInputObject::CommandType::ADD_FLOATING);

		MariaTask* toAdd = taskManager->addTask(input->getTitle(), NULL, NULL);
		if (toAdd != NULL) {
			fileManager->writeFile(taskManager->getAllTasks());
		}
		Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 1);

		//edit UniqueTask101 change title UnqiueTask202'	
		input = interpreter->parseInput(inputString[1]);
		taskManager->findTask("UniqueTask101")[0]->setTitle("UniqueTask202");
		fileManager->writeFile(taskManager->getAllTasks());

		Assert::IsTrue(taskManager->findTask("UniqueTask202").size() == 1);

		//delete UnqiueTask101	
		input = interpreter->parseInput(inputString[2]);
		Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 0);

		//delete UnqiueTask202
		input = interpreter->parseInput(inputString[3]);
		taskManager->archiveTask(taskManager->findTask("UniqueTask202")[0]);
		fileManager->writeFile(taskManager->getAllTasks());

		Assert::IsTrue(taskManager->findTask("").size() == 0);

		delete fileManager;
		delete taskManager;

	}






	};
}