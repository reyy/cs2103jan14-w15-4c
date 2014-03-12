#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(Interpreter_ValidityTest) {
	public:
		TEST_METHOD(Interpreter_Valid_EmptyString) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "";

			Assert::IsFalse(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_Create) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create";

			Assert::IsFalse(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_CreateFloatingTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create Meeting with John";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_CreateDeadlineTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create Meeting with John by 14/02/14 13:00";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_CreateTimedTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "create Meeting with John from 14/02/14 13:00 to 14/02/14 14:00";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_Edit) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "edit";

			Assert::IsFalse(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_Show) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "show";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_DeleteWithoutTitle) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "delete";

			Assert::IsFalse(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_DeleteWithTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "delete Meeting with John";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}

		TEST_METHOD(Interpreter_Valid_Exit) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "exit";

			Assert::IsTrue(program->checkInputValidity(input));

			delete program;
		}
	};
}