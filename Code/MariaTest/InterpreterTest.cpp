#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(Intepreter_CommandTypeTest) {
	public:
		TEST_METHOD(Interpreter_CommandType_EmptyInput) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "";

			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::Invalid;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);

			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_CommandType_AddFloatingTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::AddFloatingTask;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);

			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_CommandType_ShowAllTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "show";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::ShowAllTask;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);
			
			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_CommandType_DeleteTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "delete";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::DeleteTask;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);
			
			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_CommandType_Exit) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "exit";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::Exit;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandType(input);
			
			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}
	};

	TEST_CLASS(Intepreter_CommandTypeRegexTest) {
	public:
		TEST_METHOD(InterpreterR_CommandType_EmptyInput) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "";

			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::Invalid;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandTypeRegex(input);
			
			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(InterpreterR_CommandType_AddFloatingTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::AddFloatingTask;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandTypeRegex(input);
			
			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(InterpreterR_CommandType_ShowAllTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "show";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::ShowAllTask;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandTypeRegex(input);
			
			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(InterpreterR_CommandType_DeleteTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "delete";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::DeleteTask;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandTypeRegex(input);
			
			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(InterpreterR_CommandType_Exit) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "exit";
			MariaInterpreter::CommandType expected = MariaInterpreter::CommandType::Exit;
			MariaInterpreter::CommandType actual;

			actual = program->getCommandTypeRegex(input);
			
			Assert::AreEqual((int)expected, (int)actual);

			// Check to see if the keyword has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}
	};

	TEST_CLASS(Intepreter_TitleTest) {
	public:
		TEST_METHOD(Interpreter_Title_EmptyInput) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "";

			string expected = "";
			string actual;

			actual = program->getTitle(input);
			Assert::AreEqual(expected, actual);

			// Check to see if the title has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_Title_OneWord) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "meeting";

			string expected = "meeting";
			string actual;

			actual = program->getTitle(input);
			Assert::AreEqual(expected, actual);

			// Check to see if the title has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_Title_TwoWords) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "family lunch";

			string expected = "family lunch";
			string actual;

			actual = program->getTitle(input);
			Assert::AreEqual(expected, actual);

			// Check to see if the title has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_Title_ThreeWords) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "dinner with girlfriend";

			string expected = "dinner with girlfriend";
			string actual;

			actual = program->getTitle(input);
			Assert::AreEqual(expected, actual);

			// Check to see if the title has been removed from the inputString
			Assert::IsTrue(input == "");

			delete program;
		}

		TEST_METHOD(Interpreter_Title_DeadlineTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "do homework by Thursday";

			string expected = "do homework";
			string actual;

			actual = program->getTitle(input);
			Assert::AreEqual(expected, actual);

			// Check to see if the title and endOfTitle keyword has been removed from the inputString
			Assert::IsTrue(input == "Thursday");

			delete program;
		}
	};
}