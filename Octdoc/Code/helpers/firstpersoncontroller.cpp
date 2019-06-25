#include "firstpersoncontroller.h"

namespace octdoc
{
	namespace hlp
	{
		FirstPersonController::FirstPersonController(mth::Position& target) :
			m_target(target),
			m_speed(1.0f),
			m_sensitivity(0.008f),
			m_sprintMul(5.0f),
			m_keyFags(0) {}
		void FirstPersonController::MouseMove(int dx, int dy)
		{
			m_target.TurnRight(dx*m_sensitivity);
			m_target.LookDown(dy*m_sensitivity);
		}
		void FirstPersonController::KeyDown(gfx::Input::KeyCode keyCode)
		{
			switch (keyCode)
			{
			case gfx::Input::KeyCode::KEY_W:
				m_keyFags |= 1 << 0;
				break;
			case gfx::Input::KeyCode::KEY_A:
				m_keyFags |= 1 << 1;
				break;
			case gfx::Input::KeyCode::KEY_S:
				m_keyFags |= 1 << 2;
				break;
			case gfx::Input::KeyCode::KEY_D:
				m_keyFags |= 1 << 3;
				break;
			case gfx::Input::KeyCode::KEY_SHIFT:
				m_keyFags |= 1 << 4;
				break;
			case gfx::Input::KeyCode::KEY_CONTROL:
				m_keyFags |= 1 << 5;
				break;
			case gfx::Input::KeyCode::KEY_SPACE:
				m_keyFags |= 1 << 6;
				break;
			}
		}
		void FirstPersonController::KeyUp(gfx::Input::KeyCode keyCode)
		{
			switch (keyCode)
			{
			case gfx::Input::KeyCode::KEY_W:
				m_keyFags &= ~(1 << 0);
				break;
			case gfx::Input::KeyCode::KEY_A:
				m_keyFags &= ~(1 << 1);
				break;
			case gfx::Input::KeyCode::KEY_S:
				m_keyFags &= ~(1 << 2);
				break;
			case gfx::Input::KeyCode::KEY_D:
				m_keyFags &= ~(1 << 3);
				break;
			case gfx::Input::KeyCode::KEY_SHIFT:
				m_keyFags &= ~(1 << 4);
				break;
			case gfx::Input::KeyCode::KEY_CONTROL:
				m_keyFags &= ~(1 << 5);
				break;
			case gfx::Input::KeyCode::KEY_SPACE:
				m_keyFags &= ~(1 << 6);
				break;
			}
		}
		void FirstPersonController::Update(float deltaTime)
		{
			float speed = m_speed * deltaTime;
			if (m_keyFags & (1 << 4))
				speed *= m_sprintMul;
			float forward = 0.0f, right = 0.0f, up = 0.0f;
			if (m_keyFags & (1 << 0))
				forward += speed;
			if (m_keyFags & (1 << 2))
				forward -= speed;
			if (m_keyFags & (1 << 1))
				right -= speed;
			if (m_keyFags & (1 << 3))
				right += speed;
			if (m_keyFags & (1 << 5))
				up -= speed;
			if (m_keyFags & (1 << 6))
				up += speed;
			m_target.MoveForward(forward);
			m_target.MoveRight(right);
			m_target.MoveUp(up);
		}
	}
}