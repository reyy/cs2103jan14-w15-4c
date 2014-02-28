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
			
			Assert::AreEqual((*tempList)[0]->getTitle(), (string)("new task"));

			delete program;
		}
	};
}