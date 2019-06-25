#include "rendertargettexture_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			RenderTargetTexture_DX11::RenderTargetTexture_DX11(Graphics_DX11& graphics, int width, int height)
			{
				m_viewPort.TopLeftX = 0.0f;
				m_viewPort.TopLeftY = 0.0f;
				m_viewPort.Width = static_cast<float>(width);
				m_viewPort.Height = static_cast<float>(height);
				m_viewPort.MinDepth = 0.0f;
				m_viewPort.MaxDepth = 1.0f;
			}
			RenderTargetTexture_DX11::P RenderTargetTexture_DX11::CreateP(Graphics_DX11& graphics, int width, int height)
			{
				return std::make_shared<RenderTargetTexture_DX11>(graphics, width, height);
			}
			RenderTargetTexture_DX11::U RenderTargetTexture_DX11::CreateU(Graphics_DX11& graphics, int width, int height)
			{
				return std::make_unique<RenderTargetTexture_DX11>(graphics, width, height);
			}
			void RenderTargetTexture_DX11::SetAsRenderTarget(Graphics& graphics)
			{
				ID3D11DeviceContext *context = ((Graphics_DX11&)graphics).getContext();
				context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
				context->RSSetViewports(1, &m_viewPort);
			}
			void RenderTargetTexture_DX11::SetToRender(Graphics& graphics, unsigned index)
			{
				ID3D11DeviceContext *context = ((Graphics_DX11&)graphics).getContext();
				context->PSSetShaderResources(index, 1, &m_shaderResourceView);
			}
		}
	}
}