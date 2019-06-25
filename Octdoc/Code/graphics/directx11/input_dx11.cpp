#include "input_dx11.h"
#include "string"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			inline MouseMoveEvent CreateMouseMoveEvent(Input& input, WPARAM wparam, LPARAM lparam)
			{
				return MouseMoveEvent(input,
					LOWORD(lparam), HIWORD(lparam),
					LOWORD(lparam) - input.getMouseX(),
					HIWORD(lparam) - input.getMouseY(),
					(bool)(wparam & MK_RBUTTON),
					(bool)(wparam & MK_LBUTTON),
					(bool)(wparam & MK_MBUTTON),
					(bool)(wparam & MK_CONTROL),
					(bool)(wparam & MK_ALT),
					(bool)(wparam & MK_SHIFT));
			}
			inline MouseButtonEvent CreateMouseButtonEvent(Input& input, WPARAM wparam, LPARAM lparam)
			{
				return MouseButtonEvent(input,
					LOWORD(lparam), HIWORD(lparam),
					(bool)(wparam & MK_RBUTTON),
					(bool)(wparam & MK_LBUTTON),
					(bool)(wparam & MK_MBUTTON),
					(bool)(wparam & MK_CONTROL),
					(bool)(wparam & MK_ALT),
					(bool)(wparam & MK_SHIFT));
			}
			inline MouseWheelEvent CreateMouseWheelEvent(Input& input, WPARAM wparam, LPARAM lparam)
			{
				return MouseWheelEvent(input,
					input.getMouseX(), input.getMouseY(),
					(GET_WHEEL_DELTA_WPARAM(wparam) > 0),
					(bool)(wparam & MK_RBUTTON),
					(bool)(wparam & MK_LBUTTON),
					(bool)(wparam & MK_MBUTTON),
					(bool)(wparam & MK_CONTROL),
					(bool)(wparam & MK_ALT),
					(bool)(wparam & MK_SHIFT));
			}
			inline KeyEvent CreateKeyEvent(Input& input, WPARAM wparam)
			{
				return KeyEvent(input, 
					(Input::KeyCode)wparam, 
					input.isPressed(Input::KeyCode::KEY_CONTROL),
					input.isPressed(Input::KeyCode::KEY_MENU), 
					input.isPressed(Input::KeyCode::KEY_SHIFT));
			}

			void Input_DX11::SetMousePosition(LPARAM lparam)
			{
				m_mouseX = LOWORD(lparam);
				m_mouseY = HIWORD(lparam);
			}
			void Input_DX11::MouseMove(WPARAM wparam, LPARAM lparam)
			{
				if (m_mouseMoveEvent)
				{
					MouseMoveEvent e = CreateMouseMoveEvent(*this, wparam, lparam);
					SetMousePosition(lparam);
					m_mouseMoveEvent(e);
				}
				else
					SetMousePosition(lparam);
			}
			void Input_DX11::MouseLBtnDown(WPARAM wparam, LPARAM lparam)
			{
				if (m_mouseLBtnDownEvent)
				{
					MouseButtonEvent e = CreateMouseButtonEvent(*this, wparam, lparam);
					m_mouseLBtnDownEvent(e);
				}
			}
			void Input_DX11::MouseLBtnUp(WPARAM wparam, LPARAM lparam)
			{
				if (m_mouseLBtnUpEvent)
				{
					MouseButtonEvent e = CreateMouseButtonEvent(*this, wparam, lparam);
					m_mouseLBtnUpEvent(e);
				}
			}
			void Input_DX11::MouseRBtnDown(WPARAM wparam, LPARAM lparam)
			{
				if (m_mouseRBtnDownEvent)
				{
					MouseButtonEvent e = CreateMouseButtonEvent(*this, wparam, lparam);
					m_mouseRBtnDownEvent(e);
				}
			}
			void Input_DX11::MouseRBtnUp(WPARAM wparam, LPARAM lparam)
			{
				if (m_mouseRBtnUpEvent)
				{
					MouseButtonEvent e = CreateMouseButtonEvent(*this, wparam, lparam);
					m_mouseRBtnUpEvent(e);
				}
			}
			void Input_DX11::MouseMBtnDown(WPARAM wparam, LPARAM lparam)
			{
				if (m_mouseMBtnDownEvent)
				{
					MouseButtonEvent e = CreateMouseButtonEvent(*this, wparam, lparam);
					m_mouseMBtnDownEvent(e);
				}
			}
			void Input_DX11::MouseMBtnUp(WPARAM wparam, LPARAM lparam)
			{
				if (m_mouseMBtnUpEvent)
				{
					MouseButtonEvent e = CreateMouseButtonEvent(*this, wparam, lparam);
					m_mouseMBtnUpEvent(e);
				}
			}
			void Input_DX11::MouseWheel(WPARAM wparam, LPARAM lparam)
			{
				if (m_mouseWheelEvent)
				{
					MouseWheelEvent e = CreateMouseWheelEvent(*this, wparam, lparam);
					m_mouseWheelEvent(e);
				}
			}
			void Input_DX11::KeyDown(WPARAM wparam)
			{
				m_keys[wparam] = true;
				if (m_keyDownEvent)
				{
					KeyEvent e = CreateKeyEvent(*this, wparam);
					m_keyDownEvent(e);
				}
			}
			void Input_DX11::KeyUp(WPARAM wparam)
			{
				m_keys[wparam] = false;
				if (m_keyUpEvent)
				{
					KeyEvent e = CreateKeyEvent(*this, wparam);
					m_keyUpEvent(e);
				}
			}
			LRESULT Input_DX11::HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
			{
				switch (msg)
				{
				case WM_MOUSEMOVE:
					MouseMove(wparam, lparam);
					return 0;
				case WM_LBUTTONDOWN:
					MouseLBtnDown(wparam, lparam);
					return 0;
				case WM_LBUTTONUP:
					MouseLBtnUp(wparam, lparam);
					return 0;
				case WM_RBUTTONDOWN:
					MouseRBtnDown(wparam, lparam);
					return 0;
				case WM_RBUTTONUP:
					MouseRBtnUp(wparam, lparam);
					return 0;
				case WM_MBUTTONDOWN:
					MouseMBtnDown(wparam, lparam);
					return 0;
				case WM_MBUTTONUP:
					MouseMBtnUp(wparam, lparam);
					return 0;
				case WM_MOUSEWHEEL:
					MouseWheel(wparam, lparam);
					return 0;
				case WM_KEYDOWN:
					KeyDown(wparam);
					return 0;
				case WM_KEYUP:
					KeyUp(wparam);
					return 0;
				case WM_DESTROY:
					PostQuitMessage(0);
					return 0;
				}
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}

			/*
	#pragma region Events

				Event_DX11::Event_DX11(UINT msg, WPARAM wparam, LPARAM lparam) :
					m_msg(msg),
					m_wparam(wparam),
					m_lparam(lparam) {}

				MouseEvent_DX11::MouseEvent_DX11(Event_DX11& event, Input& input) :
					Event_DX11(event),
					MouseEvent(input) {}
				int MouseEvent_DX11::getX() { return LOWORD(m_lparam); }
				int MouseEvent_DX11::getY() { return HIWORD(m_lparam); }
				bool MouseEvent_DX11::isCtrlPressed() { return (bool)(m_wparam & MK_CONTROL); }
				bool MouseEvent_DX11::isAltPressed() { return (bool)(m_wparam & MK_ALT); }
				bool MouseEvent_DX11::isShiftPressed() { return (bool)(m_wparam & MK_SHIFT); }
				bool MouseEvent_DX11::isLButtonPressed() { return (bool)(m_wparam & MK_LBUTTON); }
				bool MouseEvent_DX11::isMButtonPressed() { return (bool)(m_wparam & MK_MBUTTON); }
				bool MouseEvent_DX11::isRButtonPressed() { return (bool)(m_wparam & MK_RBUTTON); }

				KeyEvent_DX11::KeyEvent_DX11(Event_DX11& event, Input& input) :
					Event_DX11(event),
					KeyEvent(input) {}
				Input::KeyCode KeyEvent_DX11::getKeyCode()
				{
					return (Input::KeyCode)m_wparam;
				}

				MouseMoveEvent_DX11::MouseMoveEvent_DX11(Event_DX11& event, Input& input, int dx, int dy) :
					MouseEvent_DX11(event, input) {}
				int MouseMoveEvent_DX11::getDX() { return m_dx; }
				int MouseMoveEvent_DX11::getDY() { return m_dy; }

	#pragma endregion*/

		}
	}
}