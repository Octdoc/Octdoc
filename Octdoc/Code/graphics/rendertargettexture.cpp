#include "directx11/rendertargettexture_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		RenderTargetTexture::P RenderTargetTexture::CreateP(Graphics& graphics, int width, int height)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::RenderTargetTexture_DX11::CreateP((dx11::Graphics_DX11&)graphics, width, height);
			}
			return nullptr;
		}
		RenderTargetTexture::U RenderTargetTexture::CreateU(Graphics& graphics, int width, int height)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::RenderTargetTexture_DX11::CreateU((dx11::Graphics_DX11&)graphics, width, height);
			}
			return nullptr;
		}
	}
}