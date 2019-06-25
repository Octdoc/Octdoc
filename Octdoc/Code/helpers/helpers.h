#pragma once

#include <memory>
#include <string>
#include <iostream>
#define SMART_PTR(TYPE) public:using P = std::shared_ptr<TYPE>;using W = std::weak_ptr<TYPE>;using U = std::unique_ptr<TYPE>;private:

namespace octdoc
{
	using uint8 = unsigned char;
	using sint8 = signed char;
	using uint16 = unsigned short;
	using sint16 = signed short;
	using uint32 = unsigned int;
	using sint32 = signed int;
	using uint64 = unsigned long long;
	using sint64 = signed long long;

	namespace hlp
	{
		std::string ToString(const wchar_t* wstr);
		std::wstring ToWString(const char* str);

		bool ScanText(std::wistream& instr, const wchar_t text[]);
		void ScanLineEnd(std::wistream& instr, std::wstring& dst);

		std::wstring GetFileExtension(const wchar_t* filename);
		void DisassemblePath(const wchar_t* path, std::wstring* folder, std::wstring* bareName, std::wstring* extension);
	}
}