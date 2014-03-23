#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MariaTest {
	TEST_CLASS(Interpreter) {
	public:
		TEST_METHOD(Interpreter_EmptyInput) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "";

			MariaInputObject* output = program->parseInput(input);

			Assert::IsTrue(output->getCommandType() == MariaInputObject::CommandInvalid);

			delete output;
			output = NULL;

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_AddFloatingTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "add Lunch with Jon";
			string expectedTitle = "Lunch with Jon";

			MariaInputObject* output = program->parseInput(input);

			Assert::IsTrue(output->getCommandType() == MariaInputObject::CommandAdd);
			Assert::IsTrue(output->getAddType() == MariaInputObject::AddFloating);
			Assert::AreEqual(expectedTitle, output->getTitle());

			delete output;
			output = NULL;

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_AddDeadlineTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "add Do homework by 15/02/2014 23:59";
			string expectedTitle = "Do homework";
			MariaTime* expected = new MariaTime(2014, 2, 15, 23, 59, 0);

			MariaInputObject* output = program->parseInput(input);

			Assert::IsTrue(output->getCommandType() == MariaInputObject::CommandAdd);
			Assert::IsTrue(output->getAddType() == MariaInputObject::AddDeadline);

			Assert::AreEqual(expectedTitle, output->getTitle());

			Assert::AreEqual(expected->getYear(), output->getEndTime()->getYear());
			Assert::AreEqual(expected->getMonth(), output->getEndTime()->getMonth());
			Assert::AreEqual(expected->getDay(), output->getEndTime()->getDay());
			Assert::AreEqual(expected->getHour(), output->getEndTime()->getHour());
			Assert::AreEqual(expected->getMin(), output->getEndTime()->getMin());

			delete expected;
			expected = NULL;

			delete output;
			output = NULL;

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_AddTimedTask) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "add Go on a holiday with friends from 15/02/2014 09:00 to 17/02/2014 09:00";
			string expectedTitle = "Go on a holiday with friends";
			MariaTime* expectedStartTime = new MariaTime(2014, 2, 15, 9, 0, 0);
			MariaTime* expectedEndTime = new MariaTime(2014, 2, 17, 9, 0, 0);

			MariaInputObject* output = program->parseInput(input);

			Assert::IsTrue(output->getCommandType() == MariaInputObject::CommandAdd);
			Assert::IsTrue(output->getAddType() == MariaInputObject::AddTimed);

			Assert::AreEqual(expectedTitle, output->getTitle());

			Assert::AreEqual(expectedStartTime->getYear(), output->getStartTime()->getYear());
			Assert::AreEqual(expectedStartTime->getMonth(), output->getStartTime()->getMonth());
			Assert::AreEqual(expectedStartTime->getDay(), output->getStartTime()->getDay());
			Assert::AreEqual(expectedStartTime->getHour(), output->getStartTime()->getHour());
			Assert::AreEqual(expectedStartTime->getMin(), output->getStartTime()->getMin());

			Assert::AreEqual(expectedEndTime->getYear(), output->getEndTime()->getYear());
			Assert::AreEqual(expectedEndTime->getMonth(), output->getEndTime()->getMonth());
			Assert::AreEqual(expectedEndTime->getDay(), output->getEndTime()->getDay());
			Assert::AreEqual(expectedEndTime->getHour(), output->getEndTime()->getHour());
			Assert::AreEqual(expectedEndTime->getMin(), output->getEndTime()->getMin());

			delete output;
			output = NULL;

			delete expectedStartTime;
			expectedStartTime = NULL;

			delete expectedEndTime;
			expectedEndTime = NULL;

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_EditTitle) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "edit Do some homework changetitle Eat dinner";
			string expectedTitle = "Do some homework";
			string expectedEditField = "Eat dinner";

			MariaInputObject* output = program->parseInput(input);

			Assert::IsTrue(output->getCommandType() == MariaInputObject::CommandEdit);
			Assert::IsTrue(output->getEditType() == MariaInputObject::EditTitle);
			Assert::AreEqual(expectedTitle, output->getTitle());
			Assert::AreEqual(expectedEditField, output->getEditField());

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_EditStartTime) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "edit Lunch with James changestart 22/03/2014 1400";
			string expectedTitle = "Lunch with James";
			MariaTime* expectedEditTime = new MariaTime(2014, 3, 22, 14, 0, 0);

			MariaInputObject* output = program->parseInput(input);

			Assert::IsTrue(output->getCommandType() == MariaInputObject::CommandEdit);
			Assert::IsTrue(output->getEditType() == MariaInputObject::EditStartTime);
			Assert::AreEqual(expectedTitle, output->getTitle());
			Assert::AreEqual(expectedEditTime->getYear(), output->getEditTime()->getYear());
			Assert::AreEqual(expectedEditTime->getMonth(), output->getEditTime()->getMonth());
			Assert::AreEqual(expectedEditTime->getDay(), output->getEditTime()->getDay());
			Assert::AreEqual(expectedEditTime->getHour(), output->getEditTime()->getHour());
			Assert::AreEqual(expectedEditTime->getMin(), output->getEditTime()->getMin());

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_EditEndTime) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "edit Breakfast at Tiffany changeend 15.02.2015 1130";
			string expectedTitle = "Breakfast at Tiffany";
			MariaTime* expectedEditTime = new MariaTime(2015, 2, 15, 11, 30, 0);

			MariaInputObject* output = program->parseInput(input);

			Assert::IsTrue(output->getCommandType() == MariaInputObject::CommandEdit);
			Assert::IsTrue(output->getEditType() == MariaInputObject::EditEndTime);
			Assert::AreEqual(expectedTitle, output->getTitle());
			Assert::AreEqual(expectedEditTime->getYear(), output->getEditTime()->getYear());
			Assert::AreEqual(expectedEditTime->getMonth(), output->getEditTime()->getMonth());
			Assert::AreEqual(expectedEditTime->getDay(), output->getEditTime()->getDay());
			Assert::AreEqual(expectedEditTime->getHour(), output->getEditTime()->getHour());
			Assert::AreEqual(expectedEditTime->getMin(), output->getEditTime()->getMin());

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_ParseDate) {
			MariaInterpreter* program = new MariaInterpreter();

			int expectedYear = 2014;
			int expectedMonth = 2;
			int expectedDay = 15;
			int year, month, day;

			program->parseDate("15/02/2014", year, month, day);
			Assert::AreEqual(expectedYear, year);
			Assert::AreEqual(expectedMonth, month);
			Assert::AreEqual(expectedDay, day);

			program->parseDate("15-02-2014", year, month, day);
			Assert::AreEqual(expectedYear, year);
			Assert::AreEqual(expectedMonth, month);
			Assert::AreEqual(expectedDay, day);

			program->parseDate("15-02-2014", year, month, day);
			Assert::AreEqual(expectedYear, year);
			Assert::AreEqual(expectedMonth, month);
			Assert::AreEqual(expectedDay, day);

			program->parseDate("15/2/2014", year, month, day);
			Assert::AreEqual(expectedYear, year);
			Assert::AreEqual(expectedMonth, month);
			Assert::AreEqual(expectedDay, day);

			program->parseDate("15-2-2014", year, month, day);
			Assert::AreEqual(expectedYear, year);
			Assert::AreEqual(expectedMonth, month);
			Assert::AreEqual(expectedDay, day);

			program->parseDate("15.2.2014", year, month, day);
			Assert::AreEqual(expectedYear, year);
			Assert::AreEqual(expectedMonth, month);
			Assert::AreEqual(expectedDay, day);

			program->parseDate("15/2/14", year, month, day);
			Assert::AreEqual(expectedYear, year);
			Assert::AreEqual(expectedMonth, month);
			Assert::AreEqual(expectedDay, day);

			program->parseDate("15-2-14", year, month, day);
			Assert::AreEqual(expectedYear, year);
			Assert::AreEqual(expectedMonth, month);
			Assert::AreEqual(expectedDay, day);

			program->parseDate("15.2.14", year, month, day);
			Assert::AreEqual(expectedYear, year);
			Assert::AreEqual(expectedMonth, month);
			Assert::AreEqual(expectedDay, day);

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_ParseDateToday) {
			MariaInterpreter* program = new MariaInterpreter();

			int year, month, day;

			program->parseDate("today", year, month, day);
			Assert::AreEqual(MariaTime::getCurrentTime().getYear(), year);
			Assert::AreEqual(MariaTime::getCurrentTime().getMonth(), month);
			Assert::AreEqual(MariaTime::getCurrentTime().getDay(), day);

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_ParseDateTomorrow) {
			MariaInterpreter* program = new MariaInterpreter();

			int year, month, day;

			program->parseDate("tomorrow", year, month, day);
			Assert::AreEqual(MariaTime::getCurrentTime().getYear(), year);
			Assert::AreEqual(MariaTime::getCurrentTime().getMonth(), month);
			Assert::AreEqual(MariaTime::getCurrentTime().getDay()+1, day);

			// NOTE: Does not account for end of month scenario.

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_ParseTime) {
			MariaInterpreter* program = new MariaInterpreter();

			int expectedHour = 12;
			int expectedMin = 34;
			int hour, min;

			program->parseTime("1234", hour, min);
			Assert::AreEqual(expectedHour, hour);
			Assert::AreEqual(expectedMin, min);

			program->parseTime("12:34", hour, min);
			Assert::AreEqual(expectedHour, hour);
			Assert::AreEqual(expectedMin, min);

			program->parseTime("12.34", hour, min);
			Assert::AreEqual(expectedHour, hour);
			Assert::AreEqual(expectedMin, min);

			// Change of format
			expectedHour = 9;
			expectedMin = 37;

			program->parseTime("937", hour, min);
			Assert::AreEqual(expectedHour, hour);
			Assert::AreEqual(expectedMin, min);

			delete program;
			program = NULL;
		}

		TEST_METHOD(Interpreter_Exit) {
			MariaInterpreter* program = new MariaInterpreter();

			string input = "exit";

			MariaInputObject* output = program->parseInput(input);

			Assert::IsTrue(output->getCommandType() == MariaInputObject::CommandExit);

			delete program;
			program = NULL;
		}
	};
}