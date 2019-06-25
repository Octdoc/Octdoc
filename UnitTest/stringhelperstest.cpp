#include "CppUnitTest.h"
#include "helpers/helpers.h"
#include <string>
#include <cstring>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace octdoc::hlp;

namespace UnitTest
{
	TEST_CLASS(StringHelpers_Test)
	{
	public:
		TEST_METHOD(GetFileExtension_Test)
		{
			std::wstring ext;

			ext = GetFileExtension(L"C:/asd/123.png");
			Assert::IsTrue(ext == L".png", L"Full path");

			ext = GetFileExtension(L"\"C:/asd/123.png\"");
			Assert::IsTrue(ext == L".png", L"Quotes");

			ext = GetFileExtension(L"123.PnG");
			Assert::IsTrue(ext == L".png", L"Capital characters");

			ext = GetFileExtension(L"asd.132.png");
			Assert::IsTrue(ext == L".png", L"Multiple dots");

			ext = GetFileExtension(L"123png");
			Assert::IsTrue(ext == L"", L"No dots");
		}
		TEST_METHOD(DisassemblePath_Test)
		{
			std::wstring folder, bareName, ext;

			DisassemblePath(L"C:/asd/123.png", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"C:/asd/", L"Full path, folder");
			Assert::IsTrue(bareName == L"123", L"Full path, bare name");
			Assert::IsTrue(ext == L".png", L"Full path, extension");

			DisassemblePath(L"123.png", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"", L"No folder, folder");
			Assert::IsTrue(bareName == L"123", L"No folder, bare name");
			Assert::IsTrue(ext == L".png", L"No folder, extension");

			DisassemblePath(L"C:/asd/.png", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"C:/asd/", L"No name, folder");
			Assert::IsTrue(bareName == L"", L"No name, bare name");
			Assert::IsTrue(ext == L".png", L"No name, extension");

			DisassemblePath(L"C:/asd/123", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"C:/asd/", L"No extension, folder");
			Assert::IsTrue(bareName == L"123", L"No extension, bare name");
			Assert::IsTrue(ext == L"", L"No extension, extension");

			DisassemblePath(L"C:/asd/", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"C:/asd/", L"Folder only, folder");
			Assert::IsTrue(bareName == L"", L"Folder only, bare name");
			Assert::IsTrue(ext == L"", L"Folder only, extension");

			DisassemblePath(L"123", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"", L"Name only, folder");
			Assert::IsTrue(bareName == L"123", L"Name only, bare name");
			Assert::IsTrue(ext == L"", L"Name only, extension");

			DisassemblePath(L".png", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"", L"Extension only, folder");
			Assert::IsTrue(bareName == L"", L"Extension only, bare name");
			Assert::IsTrue(ext == L".png", L"Extension only, extension");

			DisassemblePath(L"", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"", L"Empty, folder");
			Assert::IsTrue(bareName == L"", L"Empty, bare name");
			Assert::IsTrue(ext == L"", L"Empty, extension");
		}
		TEST_METHOD(DisassemblePath_Quotes_Test)
		{
			std::wstring folder, bareName, ext;

			DisassemblePath(L"\"C:/asd/123.png\"", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"C:/asd/", L"Full path, folder");
			Assert::IsTrue(bareName == L"123", L"Full path, bare name");
			Assert::IsTrue(ext == L".png", L"Full path, extension");

			DisassemblePath(L"\"123.png\"", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"", L"No folder, folder");
			Assert::IsTrue(bareName == L"123", L"No folder, bare name");
			Assert::IsTrue(ext == L".png", L"No folder, extension");

			DisassemblePath(L"\"C:/asd/.png\"", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"C:/asd/", L"No name, folder");
			Assert::IsTrue(bareName == L"", L"No name, bare name");
			Assert::IsTrue(ext == L".png", L"No name, extension");

			DisassemblePath(L"\"C:/asd/123\"", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"C:/asd/", L"No extension, folder");
			Assert::IsTrue(bareName == L"123", L"No extension, bare name");
			Assert::IsTrue(ext == L"", L"No extension, extension");

			DisassemblePath(L"\"C:/asd/\"", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"C:/asd/", L"Folder only, folder");
			Assert::IsTrue(bareName == L"", L"Folder only, bare name");
			Assert::IsTrue(ext == L"", L"Folder only, extension");

			DisassemblePath(L"\"123\"", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"", L"Name only, folder");
			Assert::IsTrue(bareName == L"123", L"Name only, bare name");
			Assert::IsTrue(ext == L"", L"Name only, extension");

			DisassemblePath(L"\".png\"", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"", L"Extension only, folder");
			Assert::IsTrue(bareName == L"", L"Extension only, bare name");
			Assert::IsTrue(ext == L".png", L"Extension only, extension");

			DisassemblePath(L"\"\"", &folder, &bareName, &ext);
			Assert::IsTrue(folder == L"", L"Empty, folder");
			Assert::IsTrue(bareName == L"", L"Empty, bare name");
			Assert::IsTrue(ext == L"", L"Empty, extension");
		}
	};
}