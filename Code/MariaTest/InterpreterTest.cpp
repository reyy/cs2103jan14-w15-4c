#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(Interpreter_ValidityTest) {
	public:
		TEST_METHOD(Interpreter_Valid_EmptyString_F) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_Create_F) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_CreateFloatingTask_T) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create Meeting with John";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_CreateDeadlineTask_T) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create Meeting with John by 14/02/14 13:00";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_CreateDeadlineTask_F) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create Meeting with John by 14/02/14";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_CreateTimedTask_T) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create Meeting with John from 14/02/14 13:00 to 14/02/14 14:00";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_Edit_F) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "edit";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_Show_T) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "show";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_DeleteWithoutTitle_F) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "delete";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_DeleteWithTask_T) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "delete Meeting with John";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_Exit_T) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "exit";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}
	};
}