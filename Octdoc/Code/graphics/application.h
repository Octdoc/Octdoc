#pragma once

#include "graphics.h"

namespace octdoc
{
	namespace gfx
	{
		class Application
		{
		public:
			void Register(Graphics& graphics, gfx::Input& input);
			virtual void OnStart(Graphics& graphics) {}
			virtual void OnUpdate(Graphics& graphics, double deltaTime) {}
			virtual void OnMouseMove(gfx::MouseMoveEvent& e) {}
			virtual void OnMouseLBtnDown(gfx::MouseButtonEvent& e) {}
			virtual void OnMouseLBtnUp(gfx::MouseButtonEvent& e) {}
			virtual void OnMouseRBtnDown(gfx::MouseButtonEvent& e) {}
			virtual void OnMouseRBtnUp(gfx::MouseButtonEvent& e) {}
			virtual void OnMouseMBtnDown(gfx::MouseButtonEvent& e) {}
			virtual void OnMouseMBtnUp(gfx::MouseButtonEvent& e) {}
			virtual void OnMouseWheel(gfx::MouseWheelEvent& e) {}
			virtual void OnKeyDown(gfx::KeyEvent& e) {}
			virtual void OnKeyUp(gfx::KeyEvent& e) {}
			virtual void OnResize(gfx::ResizeEvent& e) {}
		};
	}
}