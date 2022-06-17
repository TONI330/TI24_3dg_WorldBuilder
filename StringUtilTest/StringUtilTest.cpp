#include "pch.h"
#include "CppUnitTest.h"

#include "../3dg_Worldbuilder/StringUtil.h"
#include "../3dg_Worldbuilder/StringUtil.cpp"
#include <iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace StringUtilTest
{
	TEST_CLASS(StringUtilTest)
	{
	public:
		
		TEST_METHOD(TestIndexOf)
		{
			std::vector<std::string> testVector = { "this", "is", "a", "test", "array" };
			std::vector<std::string> testVector2 = { "also", "test", "for", "1", "or", "2", "\\", "'s"};

			Assert::IsTrue((StringUtil::IndexOf(testVector, "is")) == 1);
			Assert::IsTrue((StringUtil::IndexOf(testVector, "array")) == 4);
			Assert::IsTrue((StringUtil::IndexOf(testVector, "not")) == -1);
			Assert::IsTrue((StringUtil::IndexOf(testVector, "\\")) == -1);

			Assert::IsTrue((StringUtil::IndexOf(testVector2, "for")) == 2);
			Assert::IsTrue((StringUtil::IndexOf(testVector2, "1")) == 3);
			Assert::IsTrue((StringUtil::IndexOf(testVector2, "2")) == 5);
			Assert::IsTrue((StringUtil::IndexOf(testVector2, "\\")) == 6);

		}

		TEST_METHOD(TestReplace)
		{
			std::string testString = "lorem ipsum replace this part / not this";
			std::string resultString =  StringUtil::replace(testString, "replace this part", "this part is replaced");
			Assert::IsTrue(resultString == "lorem ipsum this part is replaced / not this");

			std::string testString2 = "lorem ipsum replace this part / not this";
			std::string resultString2 = StringUtil::replace(testString2, "it's not there", "this part is replaced");
			Assert::IsTrue(resultString2 == "lorem ipsum replace this part / not this");
			
		}

		TEST_METHOD(TestSplit)
		{
			std::string testString = "hello there, general kenobi";
			std::vector<std::string> expected = { "h", "llo th", "r", ", g", "n", "ral k", "nobi" };

			std::vector<std::string> result = StringUtil::split(testString, "e");

			for (int i = 0; i < expected.size(); i++)
			{
				Assert::IsTrue(expected[i] == result[i]);
			}
		}

		TEST_METHOD(TestToLower)
		{
			std::string testString = "HELLO there, GENERAL KENOBI!";
			std::string expected = "hello there, general kenobi!";

			std::string result = StringUtil::toLower(testString);
			Assert::IsTrue(expected == result);
		}

		TEST_METHOD(TestCleanLine)
		{
			std::string testString = " what  a	terrible spacing   we have ";
			std::string expected = "what a terrible spacing we have";

			std::string result = StringUtil::cleanLine(testString);

			Assert::IsTrue(expected == result);
		}
	};
}
