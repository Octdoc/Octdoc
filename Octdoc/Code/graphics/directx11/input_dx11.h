#pragma once

#include "graphics/input.h"
#include <Windows.h>

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			/*class Event_DX11
			{
			protected:
				UINT m_msg;
				WPARAM m_wparam;
				LPARAM m_lparam;

			public:
				Event_DX11(UINT msg, WPARAM wparam, LPARAM lparam);
				Event_DX11(Event_DX11& other) = default;
				Event_DX11(Event_DX11&& other) = default;
			};

			class MouseEvent_DX11 :public Event_DX11, public MouseEvent
			{
			public:
				MouseEvent_DX11(Event_DX11& event, Input& input);
				virtual int getX() override;
				virtual int getY() override;
				virtual bool isCtrlPressed() override;
				virtual bool isAltPressed() override;
				virtual bool isShiftPressed() override;
				virtual bool isLButtonPressed() override;
				virtual bool isMButtonPressed() override;
				virtual bool isRButtonPressed() override;
			};

			class KeyEvent_DX11 :public Event_DX11, public KeyEvent
			{
			public:
				KeyEvent_DX11(Event_DX11& event, Input& input);
				virtual Input::KeyCode getKeyCode() override;
			};

			class MouseMoveEvent_DX11 :public MouseEvent_DX11, public MouseMoveEvent
			{
				int m_dx, m_dy;

			public:
				MouseMoveEvent_DX11(Event_DX11& event, Input& input, int dx, int dy);
				virtual int getDX() override;
				virtual int getDY() override;
			};
			class MouseDownEvent_DX11 :public MouseEvent_DX11 {};
			class MouseUpEvent_DX11 :public MouseEvent_DX11 {};

			class KeyDownEvent_DX11 :public Event_DX11, public KeyDownEvent {};
			class KeyUpEvent_DX11 :public Event_DX11, public KeyUpEvent {};
			*/

			class Input_DX11 :public Input
			{
				SMART_PTR(Input)
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

			public:
				LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
			};
		}
	}
}