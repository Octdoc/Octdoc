#pragma once

#include "graphics.h"

namespace octdoc
{
	namespace gfx
	{
		class Application
		{
		public:
			void Register(Graphics& graphics, Input& input);
			virtual void OnStart(Graphics& graphics) {}
			virtual void OnUpdate(Graphics& graphics, float deltaTime) {}
			virtual void OnMouseMove(MouseMoveEvent& e) {}
			virtual void OnMouseLBtnDown(MouseButtonEvent& e) {}
			virtual void OnMouseLBtnUp(MouseButtonEvent& e) {}
			virtual void OnMouseRBtnDown(MouseButtonEvent& e) {}
			virtual void OnMouseRBtnUp(MouseButtonEvent& e) {}
			virtual void OnMouseMBtnDown(MouseButtonEvent& e) {}
			virtual void OnMouseMBtnUp(MouseButtonEvent& e) {}
			virtual void OnMouseWheel(MouseWheelEvent& e) {}
			virtual void OnKeyDown(KeyEvent& e) {}
			virtual void OnKeyUp(KeyEvent& e) {}
		};
	}
}