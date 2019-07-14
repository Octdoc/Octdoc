#include "testarea.h"
#include <Windows.h>
#include <sstream>

#pragma comment (lib, "Octdoc.lib")

int Main(std::vector<std::wstring>& args)
{
	octdoc::gfx::GraphicsSettings settings;
	settings.resizeable = true;
	octdoc::gfx::Graphics::U graphics = octdoc::gfx::Graphics::CreateU(settings);
	TestArea().Register(*graphics, graphics->getInput());
	graphics->Run();
	return 0;
}

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR szCmdLine, INT nCmdShow)
{
	std::vector<std::wstring> args;
	std::wstringstream ss(szCmdLine);
	std::wstring str;
	while (!ss.eof())
	{
		ss >> str;
		if (!str.empty())
			args.push_back(str);
	}
	return Main(args);
}

int wmain(int argc, wchar_t* argv[])
{
	std::vector<std::wstring> args;
	for (int i = 1; i < argc; i++)
		args.push_back(argv[i]);
	return Main(args);
}