#pragma once

#include "input.h"
#include <string>

namespace octdoc
{
	namespace gfx
	{
		enum class GraphicsEngine : unsigned int { Software = 0, DirectX11 = 1, OpenGL = 2 };

		struct GraphicsSettings
		{
			std::wstring windowName;
			int width;
			int height;
			bool fullScreen;
			bool vsyncEnable;
			bool antiAliasing;
			bool multiSamplingEnable;
			int msaa;
			GraphicsEngine engine;

			GraphicsSettings();
		};

		class Graphics
		{
			SMART_PTR(Graphics)
		protected:
			GraphicsSettings m_settings;
			std::function<void(Graphics&)> m_startFunction;
			std::function<void(Graphics&, double)> m_updateFunction;

		public:
			static Graphics::P CreateP(GraphicsSettings& settings);
			static Graphics::U CreateU(GraphicsSettings& settings);

			virtual void Run() = 0;
			virtual void SetScreenAsRenderTarget() = 0;
			virtual void ClearRenderTarget() = 0;
			virtual void ClearRenderTarget(float r, float g, float b, float a = 1.0f) = 0;
			virtual void ClearRenderTarget(float *color) = 0;
			virtual void Present() = 0;

			virtual void EnableAlphaBlending(bool blend) = 0;
			//virtual void EnableZBuffer(bool enable) = 0;
			virtual void SetPrimitiveTopology_Points() = 0;
			virtual void SetPrimitiveTopology_Lines() = 0;
			virtual void SetPrimitiveTopology_Triangles() = 0;
			virtual void SetFillMode_Solid() = 0;
			virtual void SetFillMode_Wireframe() = 0;

			virtual void SetWindowTitle(const wchar_t *title) = 0;
			virtual void Quit() = 0;

			virtual Input& getInput() = 0;

			inline void setStartFunction(std::function<void(Graphics& graphics)> fn) { m_startFunction = fn; }
			inline void setUpdateFunction(std::function<void(Graphics& graphics, float deltaTime)> fn) { m_updateFunction = fn; }
			inline float GetScreenAspectRatio() { return (float)m_settings.width / (float)m_settings.height; }
			inline float getWidth() { return (float)m_settings.width; }
			inline float getHeight() { return (float)m_settings.height; }
			inline GraphicsEngine getGfxEngine() { return m_settings.engine; }
		};
	}
}