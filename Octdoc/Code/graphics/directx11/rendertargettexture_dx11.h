#pragma once

#include "graphics/rendertargettexture.h"
#include "texture_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			class RenderTargetTexture_DX11 :public  RenderTargetTexture
			{
				SMART_PTR(RenderTargetTexture_DX11)
			private:
				COM_Ptr<ID3D11Texture2D> m_renderTargetTexture;
				COM_Ptr<ID3D11ShaderResourceView> m_shaderResourceView;
				COM_Ptr<ID3D11RenderTargetView> m_renderTargetView;
				COM_Ptr<ID3D11DepthStencilView> m_depthStencilView;
				COM_Ptr<ID3D11Texture2D> m_depthBuffer;
				D3D11_VIEWPORT m_viewPort;

			protected:
				virtual void ClearRenderTarget(Graphics& graphics, float clearColor[4]) override;

			public:
				RenderTargetTexture_DX11(Graphics_DX11& graphics, int width, int height);
				static RenderTargetTexture_DX11::P CreateP(Graphics_DX11& graphics, int width, int height);
				static RenderTargetTexture_DX11::U CreateU(Graphics_DX11& graphics, int width, int height);
				virtual void SetAsRenderTarget(Graphics& graphics) override;
				virtual void SetToRender(Graphics& graphics, unsigned index = 0) override;
			};
		}
	}
}