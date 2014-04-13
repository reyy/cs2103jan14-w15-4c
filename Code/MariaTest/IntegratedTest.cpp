#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(IntegratedTest) {
	public:
		TEST_METHOD(MSSTest) {
			string inputString = "create UniqueTask101";

			MariaInterpreter *interpreter = new MariaInterpreter();
			MariaFileManager *fileManager = new MariaFileManager();
			MariaTaskManager *taskManager = new MariaTaskManager(fileManager->openFile());
			
			MariaInputObject* input = interpreter->parseInput(inputString);

			if(input->getCOMMAND_TYPE() == MariaInputObject::COMMAND_TYPE::ADD_FLOATING) {
				MariaTask *toAdd = taskManager->addTask(input->getTitle(), NULL, NULL);
				if(toAdd != NULL) {
						fileManager->writeFile(taskManager->getAllTasks());
				}

				Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 1);

				//Reload the managers to check if task is still there
				fileManager = new MariaFileManager();
				taskManager = new MariaTaskManager(fileManager->openFile());

				Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 1);

				taskManager->archiveTask(taskManager->findTask("UniqueTask101")[0]);
				fileManager->writeFile(taskManager->getAllTasks());

				Assert::IsTrue(taskManager->findTask("UniqueTask101").size() == 0);
				
			} else {
				Assert::IsTrue(false);
			}
		}


		

		

	};
}