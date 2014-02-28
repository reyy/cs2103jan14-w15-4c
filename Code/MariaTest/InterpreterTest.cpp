#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(IntepreterTest) {
	public:
		TEST_METHOD(Interpreter_EmptyInput) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::Invalid;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);

			Assert::AreEqual((int)actual, (int)expected);
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_AddFloatingTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::AddFloatingTask;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);

			Assert::AreEqual((int)actual, (int)expected);
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_ShowAllTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "show";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::ShowAllTask;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);

			Assert::AreEqual((int)actual, (int)expected);
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_DeleteTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "delete";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::DeleteTask;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);

			Assert::AreEqual((int)actual, (int)expected);
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_Exit) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "exit";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::Exit;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);

			Assert::AreEqual((int)actual, (int)expected);
			Assert::IsTrue(input == "");

			delete program;
		}
	};
}