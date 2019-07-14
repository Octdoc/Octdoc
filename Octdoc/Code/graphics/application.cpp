#include "application.h"
namespace octdoc
{
	namespace gfx
	{
		void Application::Register(Graphics& graphics, gfx::Input& input)
		{
			graphics.setStartFunction([this](Graphics& g) { OnStart(g); });
			graphics.setUpdateFunction([this](Graphics& g, double deltaTime) { OnUpdate(g, deltaTime); });
			input.setMouseMoveEvent([this](gfx::MouseMoveEvent& e) { OnMouseMove(e); });
			input.setMouseLBtnDownEvent([this](gfx::MouseButtonEvent& e) { OnMouseLBtnDown(e); });
			input.setMouseLBtnUpEvent([this](gfx::MouseButtonEvent& e) { OnMouseLBtnUp(e); });
			input.setMouseRBtnDownEvent([this](gfx::MouseButtonEvent& e) { OnMouseRBtnDown(e); });
			input.setMouseRBtnUpEvent([this](gfx::MouseButtonEvent& e) { OnMouseRBtnUp(e); });
			input.setMouseMBtnDownEvent([this](gfx::MouseButtonEvent& e) { OnMouseMBtnDown(e); });
			input.setMouseMBtnUpEvent([this](gfx::MouseButtonEvent& e) { OnMouseMBtnUp(e); });
			input.setMouseWheelEvent([this](gfx::MouseWheelEvent& e) { OnMouseWheel(e); });
			input.setKeyDownEvent([this](gfx::KeyEvent& e) { OnKeyDown(e); });
			input.setKeyUpEvent([this](gfx::KeyEvent& e) { OnKeyUp(e); });
			input.setResizeEvent([this](gfx::ResizeEvent& e) { OnResize(e); });
		}
	}
}