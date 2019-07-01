#pragma once

#include "position.hpp"
#include "graphics/input.h"

namespace octdoc
{
	namespace hlp
	{
		class FirstPersonController
		{
			mth::Position<float>& m_target;
			float m_speed;
			float m_sensitivity;
			float m_sprintMul;
			char m_keyFags;	//W_A_S_D_Shift_Ctrl_Space

		public:
			FirstPersonController(mth::Position<float>& target);
			void MouseMove(int dx, int dy);
			void KeyDown(gfx::Input::KeyCode keyCode);
			void KeyUp(gfx::Input::KeyCode keyCode);
			void Update(float deltaTime);
		};
	}
}