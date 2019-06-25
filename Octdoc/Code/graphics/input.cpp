#include "input.h"
#include <memory>

namespace octdoc
{
	namespace gfx
	{
		Input::Input()
		{
			memset(m_keys, 0, sizeof(m_keys));
			m_mouseX = 0;
			m_mouseY = 0;
		}

		MouseMoveEvent::MouseMoveEvent(Input& input, int x, int y, int dx, int dy,
			bool rBtnPressed, bool lBtnPressed, bool mBtnPressed,
			bool ctrlPressed, bool altPressed, bool shiftPressed) :
			input(input), x(x), y(y), dx(dx), dy(dy),
			rBtnPressed(rBtnPressed),
			lBtnPressed(lBtnPressed),
			mBtnPressed(mBtnPressed),
			ctrlPressed(ctrlPressed),
			altPressed(altPressed),
			shiftPressed(shiftPressed) {}

		MouseButtonEvent::MouseButtonEvent(Input& input, int x, int y,
			bool rBtnPressed, bool lBtnPressed, bool mBtnPressed,
			bool ctrlPressed, bool altPressed, bool shiftPressed) :
			input(input), x(x), y(y),
			rBtnPressed(rBtnPressed),
			lBtnPressed(lBtnPressed),
			mBtnPressed(mBtnPressed),
			ctrlPressed(ctrlPressed),
			altPressed(altPressed),
			shiftPressed(shiftPressed) {}

		MouseWheelEvent::MouseWheelEvent(
			Input& input, int x, int y, bool upWheel,
			bool rBtnPressed, bool lBtnPressed, bool mBtnPressed,
			bool ctrlPressed, bool altPressed, bool shiftPressed) :
			input(input), x(x), y(y),
			upWheel(upWheel),
			rBtnPressed(rBtnPressed),
			lBtnPressed(lBtnPressed),
			mBtnPressed(mBtnPressed),
			ctrlPressed(ctrlPressed),
			altPressed(altPressed),
			shiftPressed(shiftPressed) {}

		KeyEvent::KeyEvent(Input& input, Input::KeyCode keyCode,
			bool ctrlPressed, bool altPressed, bool shiftPressed) :
			input(input), keyCode(keyCode),
			ctrlPressed(ctrlPressed),
			altPressed(altPressed),
			shiftPressed(shiftPressed) {}

		/*InputEvent::InputEvent(Input& input) :m_input(input) {}
		bool InputEvent::isCtrlPressed()
		{
			return m_input.isPressed(Input::KeyCode::VK_CONTROL);
		}
		bool InputEvent::isAltPressed()
		{
			return m_input.isPressed(Input::KeyCode::VK_MENU);
		}
		bool InputEvent::isShiftPressed()
		{
			return m_input.isPressed(Input::KeyCode::VK_SHIFT);
		}
		MouseEvent::MouseEvent(Input& input) :
			InputEvent(input) {}
		KeyEvent::KeyEvent(Input& input) :
			InputEvent(input) {}
		MouseMoveEvent::MouseMoveEvent(Input& input) :
			MouseEvent(input) {}
		MouseLButtonDownEvent::MouseLButtonDownEvent(Input& input) :
			MouseEvent(input) {}
		MouseLButtonUpEvent::MouseLButtonUpEvent(Input& input) :
			MouseEvent(input) {}
		MouseRButtonDownEvent::MouseRButtonDownEvent(Input& input) :
			MouseEvent(input) {}
		MouseRButtonUpEvent::MouseRButtonUpEvent(Input& input) :
			MouseEvent(input) {}
		MouseMButtonDownEvent::MouseMButtonDownEvent(Input& input) :
			MouseEvent(input) {}
		MouseMButtonUpEvent::MouseMButtonUpEvent(Input& input) :
			MouseEvent(input) {}
		KeyDownEvent::KeyDownEvent(Input& input) :
			KeyEvent(input) {}
		KeyUpEvent::KeyUpEvent(Input& input) :
			KeyEvent(input) {}*/

	}
}