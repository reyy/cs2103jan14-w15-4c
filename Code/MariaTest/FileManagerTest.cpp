#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(FileManagerTest) {
	public:
		TEST_METHOD(File_LoadEmptyFile) {
			MariaFileManager* program = new MariaFileManager();
			vector<MariaTask*>* tempList = program->openFile();

			//program->writeFile();
			
			Assert::IsTrue(tempList->empty());
			//Assert::AreEqual((tempList)[0]->getTitle(), (string)("new task"));

			delete program;
		}

		TEST_METHOD(File_LoadFile) {
			MariaFileManager* program = new MariaFileManager();
			vector<MariaTask*>* tempList = program->openFile();
			
			//program->writeFile();
			
			Assert::AreEqual((*tempList)[1]->getTitle(), (string)("new task2"));

			delete program;
		}
	};
}