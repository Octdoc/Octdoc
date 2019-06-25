#include "application.h"
namespace octdoc
{
	namespace gfx
	{
		void Application::Register(Graphics& graphics, Input& input)
		{
			graphics.setStartFunction([this](Graphics& g) { OnStart(g); });
			graphics.setUpdateFunction([this](Graphics& g, float deltaTime) { OnUpdate(g, deltaTime); });
			input.setMouseMoveEvent([this](MouseMoveEvent& e) { OnMouseMove(e); });
			input.setMouseLBtnDownEvent([this](MouseButtonEvent& e) { OnMouseLBtnDown(e); });
			input.setMouseLBtnUpEvent([this](MouseButtonEvent& e) { OnMouseLBtnUp(e); });
			input.setMouseRBtnDownEvent([this](MouseButtonEvent& e) { OnMouseRBtnDown(e); });
			input.setMouseRBtnUpEvent([this](MouseButtonEvent& e) { OnMouseRBtnUp(e); });
			input.setMouseMBtnDownEvent([this](MouseButtonEvent& e) { OnMouseMBtnDown(e); });
			input.setMouseMBtnUpEvent([this](MouseButtonEvent& e) { OnMouseMBtnUp(e); });
			input.setMouseWheelEvent([this](MouseWheelEvent& e) { OnMouseWheel(e); });
			input.setKeyDownEvent([this](KeyEvent& e) { OnKeyDown(e); });
			input.setKeyUpEvent([this](KeyEvent& e) { OnKeyUp(e); });
		}
	}
}