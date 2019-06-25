#include "directx11/texture_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		Texture::P Texture::CreateP(Graphics& graphics, void* data, int width, int height)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Texture_DX11::CreateP((dx11::Graphics_DX11&)graphics, data, width, height);
			}
			return nullptr;
		}
		Texture::U Texture::CreateU(Graphics& graphics, void* data, int width, int height)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Texture_DX11::CreateU((dx11::Graphics_DX11&)graphics, data, width, height);
			}
			return nullptr;
		}
		Texture::P Texture::CreateP(Graphics& graphics, const wchar_t* filename)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Texture_DX11::CreateP((dx11::Graphics_DX11&)graphics, filename);
			}
			return nullptr;
		}
		Texture::U Texture::CreateU(Graphics& graphics, const wchar_t* filename)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Texture_DX11::CreateU((dx11::Graphics_DX11&)graphics, filename);
			}
			return nullptr;
		}
	}
}
