#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(TaskTest) {
	public:
		TEST_METHOD(Task_AddFloatingTask) {
			//MariaTaskManager* program = new MariaTaskManager();
			//
			//program->addTask("new task");
			//vector<MariaTask*> *tempList = program->taskList;
			//
			////Assert::AreEqual((*tempList)[0]->getTitle(), (string)("new task"));

			//delete program;
			
			MariaTime a = MariaTime::getCurrentTime();
			for(int i=0; i<999999999; i++){}
			Assert::AreEqual(-1,a.compareTo(MariaTime::getCurrentTime()));
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

		TEST_METHOD(Task_GetAllTask) {
			MariaTaskManager* program = new MariaTaskManager();
			//program->taskList = NULL;
			program->getAllTasks();
			Assert::IsTrue(true);
		}

		TEST_METHOD(Task_Sort) {
			MariaTaskManager* program = new MariaTaskManager();
			program->addTask("bbb");
			program->addTask("aaa");
			program->addTask("1",NULL,&MariaTime::getCurrentTime());
			program->addTask("ccc");
			program->addTask("123");
			program->sortTasks();
			Assert::AreEqual((*program->taskList)[4]->getTitle(),(string)"1");
		}

		TEST_METHOD(Task_UndoOnce) {
			MariaTaskManager* program = new MariaTaskManager();
			program->addTask("Original Title");
			program->addUndo((*program->taskList)[0]);
			(*program->taskList)[0]->setTitle("New Title");
			program->undoLast();
			Assert::AreEqual((string)"Original Title",(*program->taskList)[0]->getTitle());
		}

		TEST_METHOD(Task_UndoMultiple) {
			MariaTaskManager* program = new MariaTaskManager();
			program->addTask("Original Title");

			program->addUndo((*program->taskList)[0]);
			(*program->taskList)[0]->setTitle("New Title 1");

			program->addUndo((*program->taskList)[0]);
			(*program->taskList)[0]->setTitle("New Title 2");

			program->undoLast();
			Assert::AreEqual((string)"New Title 1",(*program->taskList)[0]->getTitle());
			program->undoLast();
			Assert::AreEqual((string)"Original Title",(*program->taskList)[0]->getTitle());
		}

		TEST_METHOD(Task_UndoDelete) {
			MariaTaskManager* program = new MariaTaskManager();
			program->addTask("Original Title");
			MariaTask * temp = program->findTask("Original Title")[0];

			program->addUndo(temp/*(*program->taskList)[0]*/);
			temp/*(*program->taskList)[0]*/->setTitle("New Title 1");

			program->addUndo(temp/*(*program->taskList)[0]*/);
			program->archiveTask((*program->taskList)[0]);

			program->undoLast();
			Assert::AreEqual((string)"New Title 1",(*program->taskList)[0]->getTitle());
			program->undoLast();
			Assert::AreEqual((string)"Original Title",(*program->taskList)[0]->getTitle());
		}
	};
}