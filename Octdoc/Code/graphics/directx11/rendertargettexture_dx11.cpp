#include "rendertargettexture_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			void RenderTargetTexture_DX11::ClearRenderTarget(Graphics& graphics, float clearColor[4])
			{
				ID3D11DeviceContext* context = ((Graphics_DX11&)graphics).getContext();
				context->ClearRenderTargetView(m_renderTargetView, clearColor);
				context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
			}
			RenderTargetTexture_DX11::RenderTargetTexture_DX11(Graphics_DX11& graphics, int width, int height)
			{
				ID3D11Device* device = graphics.getDevice();
				HRESULT hr;

				D3D11_TEXTURE2D_DESC textureDesc{};
				textureDesc.Width = width;
				textureDesc.Height = height;
				textureDesc.MipLevels = 1;
				textureDesc.ArraySize = 1;
				textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
				textureDesc.SampleDesc.Count = 1;
				textureDesc.SampleDesc.Quality = 0;
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
				textureDesc.CPUAccessFlags = 0;
				textureDesc.MiscFlags = 0;
				hr = device->CreateTexture2D(&textureDesc, nullptr, &m_renderTargetTexture);
				if (FAILED(hr))
					throw std::exception("Failed to create texture");

				D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc{};
				shaderResourceViewDesc.Format = textureDesc.Format;
				shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				shaderResourceViewDesc.Texture2D.MipLevels = 1;
				shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
				hr = device->CreateShaderResourceView(m_renderTargetTexture, &shaderResourceViewDesc, &m_shaderResourceView);
				if (FAILED(hr))
					throw std::exception("Failed to create shader resource view");

				D3D11_TEXTURE2D_DESC depthBufferDesc;
				depthBufferDesc.Width = width;
				depthBufferDesc.Height = height;
				depthBufferDesc.MipLevels = 1;
				depthBufferDesc.ArraySize = 1;
				depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				depthBufferDesc.SampleDesc.Count = 1;
				depthBufferDesc.SampleDesc.Quality = 0;
				depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				depthBufferDesc.CPUAccessFlags = 0;
				depthBufferDesc.MiscFlags = 0;
				hr = device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthBuffer);
				if (FAILED(hr))
					throw std::exception("Failed to create back buffer");

				D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
				depthStencilViewDesc.Format = depthBufferDesc.Format;
				depthStencilViewDesc.Flags = 0;
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDesc.Texture2D.MipSlice = 0;
				hr = device->CreateDepthStencilView(m_depthBuffer, &depthStencilViewDesc, &m_depthStencilView);
				if (FAILED(hr))
					throw std::exception("Failed to create depth stencil view");

				D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
				renderTargetViewDesc.Format = textureDesc.Format;
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc.Texture2D.MipSlice = 0;
				hr = device->CreateRenderTargetView(m_renderTargetTexture, &renderTargetViewDesc, &m_renderTargetView);
				if (FAILED(hr))
					throw std::exception("Failed to create render target view");

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
				ID3D11DeviceContext* context = ((Graphics_DX11&)graphics).getContext();
				context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
				context->RSSetViewports(1, &m_viewPort);
			}
			void RenderTargetTexture_DX11::SetToRender(Graphics& graphics, unsigned index)
			{
				ID3D11DeviceContext* context = ((Graphics_DX11&)graphics).getContext();
				context->PSSetShaderResources(index, 1, &m_shaderResourceView);
			}
		}
	}
}