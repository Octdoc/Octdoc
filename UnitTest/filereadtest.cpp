#include "CppUnitTest.h"
#include "helpers/helpers.h"
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace octdoc::hlp;

namespace UnitTest
{
	TEST_CLASS(ScanText_Test)
	{
	public:
		TEST_METHOD(EqualsTest)
		{
			std::wstringstream instr(L"asd 123");
			Assert::IsTrue(ScanText(instr, L"asd 123"));
		}
		TEST_METHOD(NotEqualsTest)
		{
			std::wstringstream instr(L"123 as");
			Assert::IsFalse(ScanText(instr, L"123 d"));
		}
		TEST_METHOD(ShorterInputStreamTest)
		{
			std::wstringstream instr(L"123 asd");
			Assert::IsFalse(ScanText(instr, L"123 asd fgh"));
		}
		TEST_METHOD(LongerInputStreamTest)
		{
			std::wstringstream instr(L"123 asd fgh");
			Assert::IsTrue(ScanText(instr, L"123 asd"));
			Assert::IsTrue(ScanText(instr, L"fgh"));
		}
	};
}