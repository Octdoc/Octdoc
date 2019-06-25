#include "directx11/samplerstate_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		SamplerState::P SamplerState::CreateP(Graphics& graphics, bool interpolate, bool repeatedWrap)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::SamplerState_DX11::CreateP(((dx11::Graphics_DX11&)graphics), interpolate, repeatedWrap);
			}
			return nullptr;
		}
		SamplerState::U SamplerState::CreateU(Graphics& graphics, bool interpolate, bool repeatedWrap)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::SamplerState_DX11::CreateU(((dx11::Graphics_DX11&)graphics), interpolate, repeatedWrap);
			}
			return nullptr;
		}
	}
}
