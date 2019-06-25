#include "testarea.h"
#include <Windows.h>

//INT WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, INT)
int main()
{
	octdoc::gfx::GraphicsSettings settings;
	octdoc::gfx::Graphics::U graphics = octdoc::gfx::Graphics::CreateU(settings);
	TestArea().Register(*graphics, graphics->getInput());
	graphics->Run();
	return 0;
}