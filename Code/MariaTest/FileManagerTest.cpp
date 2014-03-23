#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(FileManagerTest) {
	public:

		TEST_METHOD(File_LoadFile) {
			MariaFileManager* program = new MariaFileManager();
			vector<MariaTask*>* tempList = program->openFile();
			
			//program->writeFile();
			
			Assert::AreEqual((*tempList)[1]->getTitle(), (string)("new task2"));

			delete program;
		}
	};
}