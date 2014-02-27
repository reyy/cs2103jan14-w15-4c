#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest
{		
	TEST_CLASS(IntepreterTest)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			// TODO: Your test code here
			MariaInterpreter* program = new MariaInterpreter();

			program->setApple(5);

			int actual = program->getApple();
			int expected = 5;

			Assert::AreEqual(actual, expected);

			delete program;
		}

	};
}