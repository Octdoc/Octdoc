#include "samplerstate_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			SamplerState_DX11::SamplerState_DX11(Graphics_DX11& graphics, bool interpolate, bool repeatedWarp)
			{
				HRESULT hr;
				D3D11_SAMPLER_DESC sd;
				sd.Filter = interpolate ? D3D11_FILTER_MIN_MAG_MIP_LINEAR : D3D11_FILTER_MIN_MAG_MIP_POINT;
				if (repeatedWarp)
				{
					sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
					sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
					sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
				}
				else
				{
					sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
					sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
					sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
				}
				sd.MipLODBias = 0.0f;
				sd.MaxAnisotropy = 1;
				sd.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
				sd.BorderColor[0] = 0.0f;
				sd.BorderColor[1] = 0.0f;
				sd.BorderColor[2] = 0.0f;
				sd.BorderColor[3] = 0.0f;
				sd.MinLOD = 0;
				sd.MaxLOD = D3D11_FLOAT32_MAX;

				hr = graphics.getDevice()->CreateSamplerState(&sd, &m_samplerState);
				if (FAILED(hr))
					throw std::exception("Failed to create sampler state");
			}
			SamplerState_DX11::P SamplerState_DX11::CreateP(Graphics_DX11& graphics, bool interpolate, bool repeatedWrap)
			{
				return std::make_shared<SamplerState_DX11>(graphics, interpolate, repeatedWrap);
			}
			SamplerState_DX11::U SamplerState_DX11::CreateU(Graphics_DX11& graphics, bool interpolate, bool repeatedWrap)
			{
				return std::make_unique<SamplerState_DX11>(graphics, interpolate, repeatedWrap);
			}
			void SamplerState_DX11::SetToPixelShader(Graphics& graphics, unsigned index)
			{
				ID3D11DeviceContext *context = ((Graphics_DX11&)graphics).getContext();
				context->PSSetSamplers(index, 1, &m_samplerState);
			}
		}
	}
}