#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(TaskTest) {
	public:
		TEST_METHOD(Task_AddFloatingTask) {
			MariaTaskManager* program = new MariaTaskManager();
			
			program->addTask("new task");
			vector<MariaTask*> *tempList = program->taskList;
			
			//Assert::AreEqual((*tempList)[0]->getTitle(), (string)("new task"));

			delete program;
		}

		TEST_METHOD(Task_DeleteTask) {
			MariaTaskManager* program = new MariaTaskManager();
			
			MariaTask * temp = program->addTask("new task");
			MariaTask * temp2 = new MariaTask();
			
			Assert::IsFalse(program->taskList->empty());
			Assert::IsFalse(program->archiveTask(temp2));
			Assert::IsTrue(program->archiveTask(temp));
			Assert::IsTrue(program->taskList->empty());

			delete temp2;
			delete program;
		}
	};
}