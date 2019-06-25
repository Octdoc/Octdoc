#pragma once

#include "graphics/samplerstate.h"
#include "graphics_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			class SamplerState_DX11 :public SamplerState
			{
				SMART_PTR(SamplerState_DX11)
			private:
				COM_Ptr<ID3D11SamplerState> m_samplerState;

			public:
				SamplerState_DX11(Graphics_DX11& graphics, bool interpolate, bool repeatedWarp);
				static SamplerState_DX11::P CreateP(Graphics_DX11& graphics, bool interpolate, bool repeatedWrap);
				static SamplerState_DX11::U CreateU(Graphics_DX11& graphics, bool interpolate, bool repeatedWrap);

				virtual void SetToPixelShader(Graphics& graphics, unsigned index = 0) override;
			};
		}
	}
}