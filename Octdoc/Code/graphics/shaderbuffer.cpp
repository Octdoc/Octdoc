#include "directx11/shaderbuffer_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		ShaderBuffer::P ShaderBuffer::CreateP(Graphics& graphics, unsigned size)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::ShaderBuffer_DX11::CreateP((dx11::Graphics_DX11&)graphics, size);
			}
			return nullptr;
		}
		ShaderBuffer::U ShaderBuffer::CreateU(Graphics& graphics, unsigned size)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::ShaderBuffer_DX11::CreateU((dx11::Graphics_DX11&)graphics, size);
			}
			return nullptr;
		}
	}
}