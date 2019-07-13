#include "directx11/texture_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		Texture::Texture() :m_width(0), m_height(0) {}
		Texture::~Texture() {}
		Texture::P Texture::CreateP(Graphics& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Texture_DX11::CreateP((dx11::Graphics_DX11&)graphics, data, width, height, mipLevels);
			}
			return nullptr;
		}
		Texture::U Texture::CreateU(Graphics& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Texture_DX11::CreateU((dx11::Graphics_DX11&)graphics, data, width, height, mipLevels);
			}
			return nullptr;
		}
		Texture::P Texture::CreateP(Graphics& graphics, const wchar_t* filename, unsigned mipLevels)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Texture_DX11::CreateP((dx11::Graphics_DX11&)graphics, filename, mipLevels);
			}
			return nullptr;
		}
		Texture::U Texture::CreateU(Graphics& graphics, const wchar_t* filename, unsigned mipLevels)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Texture_DX11::CreateU((dx11::Graphics_DX11&)graphics, filename, mipLevels);
			}
			return nullptr;
		}
	}
}
