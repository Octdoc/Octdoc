#include "helpers.h"
#include <codecvt>
#include <sstream>

namespace octdoc
{
	namespace hlp
	{
		std::string ToString(const wchar_t* wstr)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
			return conv.to_bytes(wstr);
		}

		std::wstring ToWString(const char* str)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conv;
			return conv.from_bytes(str);
		}
		bool ScanText(std::wistream& instr, const wchar_t text[])
		{
			std::wstringstream ss(text);
			std::wstring b1, b2;
			do {
				if (instr.eof())
					return false;
				ss >> b1;
				instr >> b2;
				if (b1 != b2)
					return false;
			} while (!ss.eof());
			return true;
		}
		void ScanLineEnd(std::wistream& instr, std::wstring& dst)
		{
			wchar_t buffer[256];
			instr.getline(buffer, 256);
			size_t offset;
			for (offset = 0; isspace(buffer[offset]); offset++);
			dst = buffer + offset;
		}
		std::wstring GetFileExtension(const wchar_t* filename)
		{
			std::wstring ext;
			int lastDotIndex = -1;
			for (int i = 0; filename[i]; i++)
				if (filename[i] == '.')
					lastDotIndex = i;
			if (lastDotIndex > 0)
			{
				while (filename[lastDotIndex] != '\0' && filename[lastDotIndex] != '\"')
				{
					wchar_t ch = filename[lastDotIndex++];
					if (ch >= 'A' && ch <= 'Z')
						ch = ch - 'A' + 'a';
					ext += ch;
				}
			}
			return ext;
		}
		void DisassemblePath(const wchar_t* path, std::wstring* folder, std::wstring* bareName, std::wstring* extension)
		{
			int lastSlash = -1;
			int lastDot = -1;
			int length;
			const wchar_t* pathNoSlash = path[0] == '\"' ? path + 1 : path;
			for (length = 0; pathNoSlash[length] != '\0' && pathNoSlash[length] != '\"'; length++)
			{
				if (pathNoSlash[length] == '/' || pathNoSlash[length] == '\\') lastSlash = length;
				else if (pathNoSlash[length] == '.') lastDot = length;
			}
			if (lastDot < 0) lastDot = length;
			if (folder)* folder = std::wstring(pathNoSlash, lastSlash + 1);
			if (bareName)* bareName = std::wstring(pathNoSlash + lastSlash + 1, lastDot - lastSlash - 1);
			if (extension)* extension = std::wstring(pathNoSlash + lastDot, length - lastDot);
		}
	}
}