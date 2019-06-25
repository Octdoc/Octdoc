#include "directx11/graphics_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		GraphicsSettings::GraphicsSettings() :
			windowName(L"Octdoc"),
			width(1280),
			height(720),
			fullScreen(false),
			vsyncEnable(true),
			antiAliasing(true), 
			multiSamplingEnable(true),
			msaa(4),
			engine(GraphicsEngine::DirectX11) {}

		Graphics::P Graphics::CreateP(GraphicsSettings& settings)
		{
			switch (settings.engine)
			{
			case GraphicsEngine::DirectX11:
				return dx11::Graphics_DX11::CreateP(settings);
			}
			return nullptr;
		}
		Graphics::U Graphics::CreateU(GraphicsSettings& settings)
		{
			switch (settings.engine)
			{
			case GraphicsEngine::DirectX11:
				return dx11::Graphics_DX11::CreateU(settings);
			}
			return nullptr;
		}
	}
}