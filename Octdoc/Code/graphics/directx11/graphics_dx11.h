#pragma once
#include "graphics/graphics.h"
#include "input_dx11.h"
#include "comptr.h"
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace octdoc 
{
	namespace gfx 
	{
		namespace dx11
		{
			class Graphics_DX11 :public Graphics
			{
				SMART_PTR(Graphics_DX11)
			private:
				HWND m_hwnd;
				Input_DX11 m_input;

				COM_Ptr<ID3D11Device> m_device;
				COM_Ptr<ID3D11DeviceContext> m_context;
				COM_Ptr<IDXGISwapChain> m_swapChain;
				COM_Ptr<ID3D11RenderTargetView> m_renderTargetView;
				COM_Ptr<ID3D11Texture2D> m_depthBuffer;
				COM_Ptr<ID3D11DepthStencilView> m_depthStencilView;
				COM_Ptr<ID3D11RasterizerState> m_rasterizerWireframe;
				COM_Ptr<ID3D11RasterizerState> m_rasterizerSolid;
				COM_Ptr<ID3D11BlendState> m_blendState_alphaOn;
				COM_Ptr<ID3D11BlendState> m_blendState_alphaOff;
				D3D11_VIEWPORT m_viewPort;

			private:
				void CreateGraphicsWindow();
				void CreateDevice();
				void CreateSwapChain();
				void CreateRenderTarget();
				void SetViewPort();
				void CreateRasterizerStates();
				void CreateBlendStates();

			public:
				void Init(GraphicsSettings& settings);
				LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

				virtual void Run() override;
				static Graphics_DX11::P CreateP(GraphicsSettings& settings);
				static Graphics_DX11::U CreateU(GraphicsSettings& settings);

				virtual void SetScreenAsRenderTarget() override;
				virtual void ClearRenderTarget() override;
				virtual void ClearRenderTarget(float r, float g, float b, float a = 1.0f) override;
				virtual void ClearRenderTarget(float *color) override;
				virtual void Present() override;

				virtual void EnableAlphaBlending(bool blend) override;
				//virtual void EnableZBuffer(bool enable) override;
				virtual void SetPrimitiveTopology_Points() override;
				virtual void SetPrimitiveTopology_Lines() override;
				virtual void SetPrimitiveTopology_Triangles() override;
				virtual void SetFillMode_Solid() override;
				virtual void SetFillMode_Wireframe() override;
				virtual void SetWindowTitle(const wchar_t *title) override;
				virtual void Quit() override;

				virtual Input& getInput() override;

				inline ID3D11DeviceContext* getContext() { return m_context; }
				inline ID3D11Device* getDevice() { return m_device; }
			};
		}
	}
}