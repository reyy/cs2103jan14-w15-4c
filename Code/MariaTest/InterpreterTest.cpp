//@author A0111821X
#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(Interpreter_New) {
	public:
		TEST_METHOD(Interpreter_AddFloatingTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "add Buy apples";
			string expectedTitle = "Buy apples";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::ADD_FLOATING);
			Assert::AreEqual(expectedTitle, output->getTitle());

			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_AddDeadlineTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "add Buy apples by today";
			string expectedTitle = "Buy apples";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::ADD_DEADLINE);

			MariaTime* expectedTime = new MariaTime(MariaTime::getCurrentTime());

			Assert::AreEqual(expectedTitle, output->getTitle());
			Assert::AreEqual(expectedTime->getYear(), output->getEndTime()->getYear());
			Assert::AreEqual(expectedTime->getMonth(), output->getEndTime()->getMonth());
			Assert::AreEqual(expectedTime->getDay(), output->getEndTime()->getDay());

			SAFE_DELETE(expectedTime);
			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_AddTimedTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "add Buy apples from today to tomorrow";
			string expectedTitle = "Buy apples";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::ADD_TIMED);
			Assert::AreEqual(expectedTitle, output->getTitle());

			MariaTime* expectedStartTime = new MariaTime(MariaTime::getCurrentTime());
			Assert::AreEqual(expectedStartTime->getYear(), output->getStartTime()->getYear());
			Assert::AreEqual(expectedStartTime->getMonth(), output->getStartTime()->getMonth());
			Assert::AreEqual(expectedStartTime->getDay(), output->getStartTime()->getDay());
			Assert::AreEqual(expectedStartTime->getDayWeek(), output->getStartTime()->getDayWeek());

			MariaTime* expectedEndTime = new MariaTime(expectedStartTime->getYear(), expectedStartTime->getMonth(), expectedStartTime->getDay()+1);
			Assert::AreEqual(expectedEndTime->getYear(), output->getEndTime()->getYear());
			Assert::AreEqual(expectedEndTime->getMonth(), output->getEndTime()->getMonth());
			Assert::AreEqual(expectedEndTime->getDay(), output->getEndTime()->getDay());
			Assert::AreEqual(expectedEndTime->getDayWeek(), output->getEndTime()->getDayWeek());

			SAFE_DELETE(expectedStartTime);
			SAFE_DELETE(expectedEndTime);
			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_EditTitle) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "edit apple change title banana";
			string expectedTitle = "apple";
			string expectedEditField = "banana";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::EDIT_TITLE);
			Assert::AreEqual(expectedTitle, output->getTitle());
			Assert::AreEqual(expectedEditField, output->getEditField());

			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_EditStartTime) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "edit apple change start 21/03/2014";
			string expectedTitle = "apple";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::EDIT_START_TIME);

			MariaTime* expectedTime = new MariaTime(2014, 3, 21);

			Assert::AreEqual(expectedTitle, output->getTitle());
			Assert::AreEqual(expectedTime->getYear(), output->getEditTime()->getYear());
			Assert::AreEqual(expectedTime->getMonth(), output->getEditTime()->getMonth());
			Assert::AreEqual(expectedTime->getDay(), output->getEditTime()->getDay());

			SAFE_DELETE(expectedTime);
			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_EditEndTime) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "edit apple change end 21/03/2014";
			string expectedTitle = "apple";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::EDIT_END_TIME);

			MariaTime* expectedTime = new MariaTime(2014, 3, 21);

			Assert::AreEqual(expectedTitle, output->getTitle());
			Assert::AreEqual(expectedTime->getYear(), output->getEditTime()->getYear());
			Assert::AreEqual(expectedTime->getMonth(), output->getEditTime()->getMonth());
			Assert::AreEqual(expectedTime->getDay(), output->getEditTime()->getDay());

			SAFE_DELETE(expectedTime);
			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_EditDescription) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "edit apple change description it is a fruit";
			string expectedTitle = "apple";
			string expectedEditField = "it is a fruit";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::EDIT_DESCRIPTION);
			Assert::AreEqual(expectedTitle, output->getTitle());
			Assert::AreEqual(expectedEditField, output->getEditField());

			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_ShowToday) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "show";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			MariaTime* expectedTime = new MariaTime(MariaTime::getCurrentTime());

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::SHOW_DATE);
			Assert::AreEqual(expectedTime->getYear(), output->getEndTime()->getYear());
			Assert::AreEqual(expectedTime->getMonth(), output->getEndTime()->getMonth());
			Assert::AreEqual(expectedTime->getDay(), output->getEndTime()->getDay());

			SAFE_DELETE(expectedTime);
			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_ShowTomorrow) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "show tomorrow";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			MariaTime* expectedTime = new MariaTime(MariaTime::getCurrentTime().getYear(), MariaTime::getCurrentTime().getMonth(), MariaTime::getCurrentTime().getDay()+1);

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::SHOW_DATE);
			Assert::AreEqual(expectedTime->getYear(), output->getEndTime()->getYear());
			Assert::AreEqual(expectedTime->getMonth(), output->getEndTime()->getMonth());
			Assert::AreEqual(expectedTime->getDay(), output->getEndTime()->getDay());

			SAFE_DELETE(expectedTime);
			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_ShowDateRange) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "show from today to tomorrow";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::SHOW_DATE_RANGE);
			MariaTime* expectedStartTime = new MariaTime(MariaTime::getCurrentTime());
			Assert::AreEqual(expectedStartTime->getYear(), output->getStartTime()->getYear());
			Assert::AreEqual(expectedStartTime->getMonth(), output->getStartTime()->getMonth());
			Assert::AreEqual(expectedStartTime->getDay(), output->getStartTime()->getDay());
			Assert::AreEqual(expectedStartTime->getDayWeek(), output->getStartTime()->getDayWeek());

			MariaTime* expectedEndTime = new MariaTime(expectedStartTime->getYear(), expectedStartTime->getMonth(), expectedStartTime->getDay()+1);
			Assert::AreEqual(expectedEndTime->getYear(), output->getEndTime()->getYear());
			Assert::AreEqual(expectedEndTime->getMonth(), output->getEndTime()->getMonth());
			Assert::AreEqual(expectedEndTime->getDay(), output->getEndTime()->getDay());
			Assert::AreEqual(expectedEndTime->getDayWeek(), output->getEndTime()->getDayWeek());

			SAFE_DELETE(expectedStartTime);
			SAFE_DELETE(expectedEndTime);
			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_SearchTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "find this is a really long long long task title";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			string expectedTitle = "this is a really long long long task title";

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::SEARCH);
			Assert::AreEqual(expectedTitle, output->getTitle());

			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_DeleteTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "delete apple";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			string expectedTitle = "apple";

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::DELETE_TASK);
			Assert::AreEqual(expectedTitle, output->getTitle());

			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_DeleteAllTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "delete all";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			string expectedTitle = "";

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::DELETE_ALL);
			Assert::AreEqual(expectedTitle, output->getTitle());

			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_MarkDone) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "done gym";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			string expectedTitle = "gym";

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::MARK_DONE);
			Assert::AreEqual(expectedTitle, output->getTitle());

			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_MarkUndone) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "undone gym";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			string expectedTitle = "gym";

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::MARK_UNDONE);
			Assert::AreEqual(expectedTitle, output->getTitle());

			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}

		TEST_METHOD(Interpreter_Undo) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "undo";

			MariaInputObject* output;
			try {
				output = program->parseInput(input);
			} catch (exception& e) {
				Assert::AreEqual("", e.what());
			}

			Assert::IsTrue(output->getCommandType() == MariaInputObject::COMMAND_TYPE::UNDO);

			SAFE_DELETE(output);
			SAFE_DELETE(program);
		}
	};
}