#pragma once

#include "graphics/input.h"
#include <Windows.h>

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			class Input_DX11 :public Input
			{
				SMART_PTR(Input_DX11)
			private:
				void SetMousePosition(LPARAM lparam);
				void MouseMove(WPARAM wparam, LPARAM lparam);
				void MouseLBtnDown(WPARAM wparam, LPARAM lparam);
				void MouseLBtnUp(WPARAM wparam, LPARAM lparam);
				void MouseRBtnDown(WPARAM wparam, LPARAM lparam);
				void MouseRBtnUp(WPARAM wparam, LPARAM lparam);
				void MouseMBtnDown(WPARAM wparam, LPARAM lparam);
				void MouseMBtnUp(WPARAM wparam, LPARAM lparam);
				void MouseWheel(WPARAM wparam, LPARAM lparam);
				void KeyDown(WPARAM wparam);
				void KeyUp(WPARAM wparam);
				void Resize(LPARAM lparam);

			public:
				LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
			};
		}
	}
}