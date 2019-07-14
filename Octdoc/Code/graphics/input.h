#pragma once

#include "helpers/helpers.h"
#include <functional>

namespace octdoc
{
	namespace gfx
	{
		struct MouseMoveEvent;
		struct MouseButtonEvent;
		struct MouseWheelEvent;
		struct KeyEvent;
		struct ResizeEvent;

		class Input
		{
			SMART_PTR(Input)
		public:
			enum KeyCode :unsigned char
			{
				KEY_LBUTTON = 1,
				KEY_RBUTTON = 2,
				KEY_CANCEL = 3,
				KEY_MBUTTON = 4,
				KEY_XBUTTON1 = 5,
				KEY_XBUTTON2 = 6,
				KEY_BACK = 8,
				KEY_TAB = 9,
				KEY_CLEAR = 12,
				KEY_RETURN = 13,
				KEY_SHIFT = 16,
				KEY_CONTROL = 17,
				KEY_MENU = 18,
				KEY_PAUSE = 19,
				KEY_CAPITAL = 20,
				KEY_KANA = 21,
				KEY_HANGUEL = 21,
				KEY_HANGUL = 21,
				KEY_JUNJA = 23,
				KEY_FINAL = 24,
				KEY_HANJA = 25,
				KEY_KANJI = 25,
				KEY_ESCAPE = 27,
				KEY_CONVERT = 28,
				KEY_NONCONVERT = 29,
				KEY_ACCEPT = 30,
				KEY_MODECHANGE = 31,
				KEY_SPACE = 32,
				KEY_PRIOR = 33,
				KEY_NEXT = 34,
				KEY_END = 35,
				KEY_HOME = 36,
				KEY_LEFT = 37,
				KEY_UP = 38,
				KEY_RIGHT = 39,
				KEY_DOWN = 40,
				KEY_SELECT = 41,
				KEY_PRINT = 42,
				KEY_EXECUTE = 43,
				KEY_SNAPSHOT = 44,
				KEY_INSERT = 45,
				KEY_DELETE = 46,
				KEY_HELP = 47,
				KEY_0 = 48,
				KEY_1 = 49,
				KEY_2 = 50,
				KEY_3 = 51,
				KEY_4 = 52,
				KEY_5 = 53,
				KEY_6 = 54,
				KEY_7 = 55,
				KEY_8 = 56,
				KEY_9 = 57,
				KEY_A = 65,
				KEY_B = 66,
				KEY_C = 67,
				KEY_D = 68,
				KEY_E = 69,
				KEY_F = 70,
				KEY_G = 71,
				KEY_H = 72,
				KEY_I = 73,
				KEY_J = 74,
				KEY_K = 75,
				KEY_L = 76,
				KEY_M = 77,
				KEY_N = 78,
				KEY_O = 79,
				KEY_P = 80,
				KEY_Q = 81,
				KEY_R = 82,
				KEY_S = 83,
				KEY_T = 84,
				KEY_U = 85,
				KEY_V = 86,
				KEY_W = 87,
				KEY_X = 88,
				KEY_Y = 89,
				KEY_Z = 90,
				KEY_LWIN = 91,
				KEY_RWIN = 92,
				KEY_APPS = 93,
				KEY_SLEEP = 95,
				KEY_NUMPAD0 = 96,
				KEY_NUMPAD1 = 97,
				KEY_NUMPAD2 = 98,
				KEY_NUMPAD3 = 99,
				KEY_NUMPAD4 = 100,
				KEY_NUMPAD5 = 101,
				KEY_NUMPAD6 = 102,
				KEY_NUMPAD7 = 103,
				KEY_NUMPAD8 = 104,
				KEY_NUMPAD9 = 105,
				KEY_MULTIPLY = 106,
				KEY_ADD = 107,
				KEY_SEPARATOR = 108,
				KEY_SUBTRACT = 109,
				KEY_DECIMAL = 110,
				KEY_DIVIDE = 111,
				KEY_F1 = 112,
				KEY_F2 = 113,
				KEY_F3 = 114,
				KEY_F4 = 115,
				KEY_F5 = 116,
				KEY_F6 = 117,
				KEY_F7 = 118,
				KEY_F8 = 119,
				KEY_F9 = 120,
				KEY_F10 = 121,
				KEY_F11 = 122,
				KEY_F12 = 123,
				KEY_F13 = 124,
				KEY_F14 = 125,
				KEY_F15 = 126,
				KEY_F16 = 127,
				KEY_F17 = 128,
				KEY_F18 = 129,
				KEY_F19 = 130,
				KEY_F20 = 131,
				KEY_F21 = 132,
				KEY_F22 = 133,
				KEY_F23 = 134,
				KEY_F24 = 135,
				KEY_NUMLOCK = 144,
				KEY_SCROLL = 145,
				KEY_LSHIFT = 160,
				KEY_RSHIFT = 161,
				KEY_LCONTROL = 162,
				KEY_RCONTROL = 163,
				KEY_LMENU = 164,
				KEY_RMENU = 165,
				KEY_BROWSER_BACK = 166,
				KEY_BROWSER_FORWARD = 167,
				KEY_BROWSER_REFRESH = 168,
				KEY_BROWSER_STOP = 169,
				KEY_BROWSER_SEARCH = 170,
				KEY_BROWSER_FAVORITES = 171,
				KEY_BROWSER_HOME = 172,
				KEY_VOLUME_MUTE = 173,
				KEY_VOLUME_DOWN = 174,
				KEY_VOLUME_UP = 175,
				KEY_MEDIA_NEXT_TRACK = 176,
				KEY_MEDIA_PREV_TRACK = 177,
				KEY_MEDIA_STOP = 178,
				KEY_MEDIA_PLAY_PAUSE = 179,
				KEY_LAUNCH_MAIL = 180,
				KEY_LAUNCH_MEDIA_SELECT = 181,
				KEY_LAUNCH_APP1 = 182,
				KEY_LAUNCH_APP2 = 183,
				KEY_OEM_1 = 186,
				KEY_OEM_PLUS = 187,
				KEY_OEM_COMMA = 188,
				KEY_OEM_MINUS = 189,
				KEY_OEM_PERIOD = 190,
				KEY_OEM_2 = 191,
				KEY_OEM_3 = 192,
				KEY_OEM_4 = 219,
				KEY_OEM_5 = 220,
				KEY_OEM_6 = 221,
				KEY_OEM_7 = 222,
				KEY_OEM_8 = 223,
				KEY_OEM_102 = 226,
				KEY_PROCESSKEY = 229,
				KEY_PACKET = 231,
				KEY_ATTN = 246,
				KEY_CRSEL = 247,
				KEY_EXSEL = 248,
				KEY_EREOF = 249,
				KEY_PLAY = 250,
				KEY_ZOOM = 251,
				KEY_NONAME = 252,
				KEY_PA1 = 253,
				KEY_OEM_CLEAR = 254
			};

		protected:
			bool m_keys[256];
			int m_mouseX, m_mouseY;

			std::function<void(MouseMoveEvent& e)> m_mouseMoveEvent;
			std::function<void(MouseButtonEvent& e)> m_mouseLBtnDownEvent;
			std::function<void(MouseButtonEvent& e)> m_mouseLBtnUpEvent;
			std::function<void(MouseButtonEvent& e)> m_mouseRBtnDownEvent;
			std::function<void(MouseButtonEvent& e)> m_mouseRBtnUpEvent;
			std::function<void(MouseButtonEvent& e)> m_mouseMBtnDownEvent;
			std::function<void(MouseButtonEvent& e)> m_mouseMBtnUpEvent;
			std::function<void(MouseWheelEvent& e)> m_mouseWheelEvent;
			std::function<void(KeyEvent& e)> m_keyDownEvent;
			std::function<void(KeyEvent& e)> m_keyUpEvent;
			std::function<void(ResizeEvent& e)> m_resizeEvent;

		public:
			Input();
			inline bool isPressed(KeyCode key) { return m_keys[key]; }
			inline int getMouseX() { return m_mouseX; }
			inline int getMouseY() { return m_mouseY; }

			inline void setMouseMoveEvent(std::function<void(MouseMoveEvent&)> e) { m_mouseMoveEvent = e; }
			inline void setMouseLBtnDownEvent(std::function<void(MouseButtonEvent&)> e) { m_mouseLBtnDownEvent = e; }
			inline void setMouseLBtnUpEvent(std::function<void(MouseButtonEvent&)> e) { m_mouseLBtnUpEvent = e; }
			inline void setMouseMBtnDownEvent(std::function<void(MouseButtonEvent&)> e) { m_mouseMBtnDownEvent = e; }
			inline void setMouseMBtnUpEvent(std::function<void(MouseButtonEvent&)> e) { m_mouseMBtnUpEvent = e; }
			inline void setMouseRBtnDownEvent(std::function<void(MouseButtonEvent&)> e) { m_mouseRBtnDownEvent = e; }
			inline void setMouseRBtnUpEvent(std::function<void(MouseButtonEvent&)> e) { m_mouseRBtnUpEvent = e; }
			inline void setMouseWheelEvent(std::function<void(MouseWheelEvent&)> e) { m_mouseWheelEvent = e; }
			inline void setKeyDownEvent(std::function<void(KeyEvent&)> e) { m_keyDownEvent = e; }
			inline void setKeyUpEvent(std::function<void(KeyEvent&)> e) { m_keyUpEvent = e; }
			inline void setResizeEvent(std::function<void(ResizeEvent&)> e) { m_resizeEvent = e; }

		};

		struct MouseMoveEvent
		{
			Input& input;
			int x, y;
			int dx, dy;
			bool rBtnPressed, lBtnPressed, mBtnPressed;
			bool ctrlPressed, altPressed, shiftPressed;

			MouseMoveEvent(Input& input, int x, int y, int dx, int  dy,
				bool rBtnPressed, bool lBtnPressed, bool mBtnPressed,
				bool ctrlPressed, bool altPressed, bool shiftPressed);
		};
		struct MouseButtonEvent
		{
			Input& input;
			int x, y;
			bool rBtnPressed, lBtnPressed, mBtnPressed;
			bool ctrlPressed, altPressed, shiftPressed;

			MouseButtonEvent(Input& input, int x, int y,
				bool rBtnPressed, bool lBtnPressed, bool mBtnPressed,
				bool ctrlPressed, bool altPressed, bool shiftPressed);
		};
		struct MouseWheelEvent
		{
			Input& input;
			int x, y;
			bool upWheel;
			bool rBtnPressed, lBtnPressed, mBtnPressed;
			bool ctrlPressed, altPressed, shiftPressed;

			MouseWheelEvent(Input& input, int x, int y, bool upWheel,
				bool rBtnPressed, bool lBtnPressed, bool mBtnPressed,
				bool ctrlPressed, bool altPressed, bool shiftPressed);
		};
		struct KeyEvent
		{
			Input& input;
			Input::KeyCode keyCode;
			bool ctrlPressed, altPressed, shiftPressed;

			KeyEvent(Input& input, Input::KeyCode keyCode,
				bool ctrlPressed, bool altPressed, bool shiftPressed);
		};
		struct ResizeEvent
		{
			Input& input;
			int width, height;

			ResizeEvent(Input& input, int width, int height);
		};

		/*class Event {};

		class InputEvent :public Event
		{
		protected:
			Input& m_input;

		public:
			InputEvent(Input& input);
			virtual bool isCtrlPressed();
			virtual bool isAltPressed();
			virtual bool isShiftPressed();
			inline Input& getInput() { return m_input; }
		};

		class MouseEvent :public InputEvent
		{
		public:
			MouseEvent(Input& input);
			virtual int getX() = 0;
			virtual int getY() = 0;
			virtual bool isLButtonPressed() = 0;
			virtual bool isMButtonPressed() = 0;
			virtual bool isRButtonPressed() = 0;
		};

		class KeyEvent :public InputEvent
		{
		public:
			KeyEvent(Input& input);
			virtual Input::KeyCode getKeyCode() = 0;
		};

		class MouseMoveEvent :public MouseEvent
		{
		public:
			MouseMoveEvent(Input& input);
			virtual int getDX() = 0;
			virtual int getDY() = 0;
		};
		class MouseLButtonDownEvent :public MouseEvent
		{
		public:
			MouseLButtonDownEvent(Input& input);
		};
		class MouseLButtonUpEvent :public MouseEvent
		{
		public:
			MouseLButtonUpEvent(Input& input);
		};
		class MouseRButtonDownEvent :public MouseEvent
		{
		public:
			MouseRButtonDownEvent(Input& input);
		};
		class MouseRButtonUpEvent :public MouseEvent
		{
		public:
			MouseRButtonUpEvent(Input& input);
		};
		class MouseMButtonDownEvent :public MouseEvent
		{
		public:
			MouseMButtonDownEvent(Input& input);
		};
		class MouseMButtonUpEvent :public MouseEvent
		{
		public:
			MouseMButtonUpEvent(Input& input);
		};

		class KeyDownEvent :public KeyEvent
		{
		public:
			KeyDownEvent(Input& input);
		};
		class KeyUpEvent :public KeyEvent
		{
		public:
			KeyUpEvent(Input& input);
		};*/
	}
}