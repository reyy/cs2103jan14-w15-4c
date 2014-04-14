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

				SAFE_DELETE(input);
				SAFE_DELETE(fileManager);
				SAFE_DELETE(taskManager);

				//Reload the managers to check if task is still there
				interpreter = new MariaInterpreter();
				fileManager = new MariaFileManager();
				taskManager = new MariaTaskManager(fileManager->openFile());

				Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 0);

				SAFE_DELETE(fileManager);
				SAFE_DELETE(taskManager);

			} else {
				Assert::IsTrue(false);
			}
		}

		//Integrated Test Case that simulates adding, editing and removing a task
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
			SAFE_DELETE(input);

			//edit UniqueTask101 change title UnqiueTask202'
			input = interpreter->parseInput(inputString[1]);
			taskManager->findTask("UniqueTask101")[0]->setTitle("UniqueTask202");
			fileManager->writeFile(taskManager->getAllTasks());

			Assert::IsTrue(taskManager->findTask("UniqueTask202").size() == 1);
			SAFE_DELETE(input);

			//delete UnqiueTask101	
			input = interpreter->parseInput(inputString[2]);
			Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 0);

			//delete UnqiueTask202
			input = interpreter->parseInput(inputString[3]);
			taskManager->archiveTask(taskManager->findTask("UniqueTask202")[0]);
			fileManager->writeFile(taskManager->getAllTasks());

			Assert::IsTrue(taskManager->findTask("").size() == 0);
			SAFE_DELETE(input);

			SAFE_DELETE(fileManager);
			SAFE_DELETE(taskManager);
		}

		//@author A0111821X
		//Integrated Test Case that simulates adding and marking a task as complete
		TEST_METHOD(MSSTest_AddMarkCompleteTask) {
			string inputString[2] = {"create UniqueTask101", 
									"done UniqueTask101"};

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
			SAFE_DELETE(input);

			//done UniqueTask101
			input = interpreter->parseInput(inputString[1]);
			taskManager->findTask(input->getTitle())[0]->setIsDone(true);
			fileManager->writeFile(taskManager->getAllTasks());

			Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 1);
			SAFE_DELETE(input);

			Assert::IsTrue(taskManager->findTask("UniqueTask101")[0]->getIsDone());

			// Do some clean up
			taskManager->archiveTask(taskManager->findTask("UniqueTask101")[0]);
			fileManager->writeFile(taskManager->getAllTasks());

			SAFE_DELETE(fileManager);
			SAFE_DELETE(taskManager);
		}

		//@author A0111784H
		//Integrated Test Case that simulates adding and finding a task successfully and unsuccessfully.
		TEST_METHOD(MSSTest_AddSearchSearchTask) {
			string inputString[3] = {"create UniqueTask101", 
									"find Unique", 
									"find Something"};

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
			SAFE_DELETE(input);

			//find Unique
			input = interpreter->parseInput(inputString[1]);
			Assert::IsTrue(taskManager->findTask(input->getTitle()).size() == 1);
			SAFE_DELETE(input);

			//find Something
			input = interpreter->parseInput(inputString[2]);
			Assert::IsTrue(taskManager->findTask(input->getTitle()).size() == 0);
			SAFE_DELETE(input);

			// Do some clean up
			taskManager->archiveTask(taskManager->findTask("UniqueTask101")[0]);
			fileManager->writeFile(taskManager->getAllTasks());

			SAFE_DELETE(fileManager);
			SAFE_DELETE(taskManager);
		}
	};
}