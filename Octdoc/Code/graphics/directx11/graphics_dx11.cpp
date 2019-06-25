#include "graphics_dx11.h"
#include <chrono>

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
			{
				return ((Graphics_DX11*)GetWindowLongPtr(hwnd, GWLP_USERDATA))->MessageHandler(hwnd, msg, wparam, lparam);
			}
			LRESULT CALLBACK InitialWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
			{
				if (msg == WM_CREATE)
				{
					SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)(((LPCREATESTRUCT)lparam)->lpCreateParams));
					SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
					WndProc(hwnd, msg, wparam, lparam);
					return 0;
				}
				return DefWindowProc(hwnd, msg, wparam, lparam);
			}

			void Graphics_DX11::Init(GraphicsSettings& settings)
			{
				m_settings = settings;
				CreateGraphicsWindow();
				CreateDevice();
				CreateSwapChain();
				CreateRenderTarget();
				SetViewPort();
				CreateRasterizerStates();
			}
			void Graphics_DX11::CreateGraphicsWindow()
			{
				WNDCLASSEX wc{};
				wc.cbSize = sizeof(wc);
				wc.hCursor = LoadCursor(NULL, IDC_ARROW);
				wc.hInstance = GetModuleHandle(NULL);
				wc.lpfnWndProc = InitialWndProc;
				wc.lpszClassName = m_settings.windowName.c_str();
				RegisterClassEx(&wc);
				RECT rect;
				rect.left = (GetSystemMetrics(SM_CXSCREEN) - m_settings.width) / 2;
				rect.top = (GetSystemMetrics(SM_CYSCREEN) - m_settings.height) / 2;
				rect.right = rect.left + m_settings.width;
				rect.bottom = rect.top + m_settings.height;
				DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
				DWORD exStyle = WS_EX_OVERLAPPEDWINDOW;
				AdjustWindowRectEx(&rect, style, FALSE, exStyle);
				m_hwnd = CreateWindowEx(exStyle, m_settings.windowName.c_str(), m_settings.windowName.c_str(), style,
					rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, wc.hInstance, (LPVOID)this);
				ShowWindow(m_hwnd, SW_SHOW);
				UpdateWindow(m_hwnd);
			}
			void Graphics_DX11::CreateDevice()
			{
				D3D_FEATURE_LEVEL featureLevel;
				HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
					/*D3D11_CREATE_DEVICE_DEBUG*/ 0, nullptr, 0, D3D11_SDK_VERSION, &m_device, &featureLevel, &m_context);
				if (FAILED(hr))
					throw std::exception("Failed to create device");
			}
			void Graphics_DX11::CreateSwapChain()
			{
				HRESULT hr;
				DXGI_SWAP_CHAIN_DESC swapChainDesc;
				swapChainDesc.BufferDesc.Width = m_settings.width;
				swapChainDesc.BufferDesc.Height = m_settings.height;
				swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
				swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
				swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
				swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
				swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				if (m_settings.multiSamplingEnable)
				{
					UINT msaaQuality;
					hr = m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, m_settings.msaa, &msaaQuality);
					if (FAILED(hr))
					{
						m_settings.multiSamplingEnable = false;
						m_settings.msaa = 1;
						swapChainDesc.SampleDesc.Count = 1;
						swapChainDesc.SampleDesc.Quality = 0;
					}
					else
					{
						swapChainDesc.SampleDesc.Count = m_settings.msaa;
						swapChainDesc.SampleDesc.Quality = msaaQuality - 1;
					}
				}
				else
				{
					swapChainDesc.SampleDesc.Count = 1;
					swapChainDesc.SampleDesc.Quality = 0;
				}
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
				swapChainDesc.BufferCount = 1;
				swapChainDesc.OutputWindow = m_hwnd;
				swapChainDesc.Windowed = !m_settings.fullScreen;
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
				swapChainDesc.Flags = 0;

				COM_Ptr<IDXGIDevice> dxgi_device;
				COM_Ptr<IDXGIAdapter> dxgi_adapter;
				COM_Ptr<IDXGIFactory> dxgi_factory;
				hr = m_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgi_device);
				if (FAILED(hr))
					throw std::exception("Failed to query DXGI device");
				hr = dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgi_adapter);
				if (FAILED(hr))
					throw std::exception("Failed to get adapter");
				hr = dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgi_factory);
				if (FAILED(hr))
					throw std::exception("Failed to get factory");

				hr = dxgi_factory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);
				if (FAILED(hr))
					throw std::exception("Failed to create swap chain");
			}
			void Graphics_DX11::CreateRenderTarget()
			{
				COM_Ptr<ID3D11Texture2D> backBuffer;
				HRESULT hr;
				hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
				if (FAILED(hr))
					throw std::exception("Failed to get back buffer");
				hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
				if (FAILED(hr))
					throw std::exception("Failed to create render target");

				D3D11_TEXTURE2D_DESC depthBufferDesc;
				depthBufferDesc.Width = m_settings.width;
				depthBufferDesc.Height = m_settings.height;
				depthBufferDesc.MipLevels = 1;
				depthBufferDesc.ArraySize = 1;
				depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
				if (m_settings.multiSamplingEnable)
				{
					UINT msaaQuality;
					hr = m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, m_settings.msaa, &msaaQuality);
					depthBufferDesc.SampleDesc.Count = m_settings.msaa;
					depthBufferDesc.SampleDesc.Quality = msaaQuality - 1;
				}
				else
				{
					depthBufferDesc.SampleDesc.Count = 1;
					depthBufferDesc.SampleDesc.Quality = 0;
				}
				depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
				depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
				depthBufferDesc.CPUAccessFlags = 0;
				depthBufferDesc.MiscFlags = 0;

				hr = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthBuffer);
				if (FAILED(hr))
					throw std::exception("Failed to create back buffer");
				hr = m_device->CreateDepthStencilView(m_depthBuffer, nullptr, &m_depthStencilView);
				if (FAILED(hr))
					throw std::exception("Failed to create depth stencil view");

				m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
			}
			void Graphics_DX11::SetViewPort()
			{
				m_viewPort.TopLeftX = 0.0f;
				m_viewPort.TopLeftY = 0.0f;
				m_viewPort.Width = static_cast<float>(m_settings.width);
				m_viewPort.Height = static_cast<float>(m_settings.height);
				m_viewPort.MinDepth = 0.0f;
				m_viewPort.MaxDepth = 1.0f;
				m_context->RSSetViewports(1, &m_viewPort);
			}
			void Graphics_DX11::CreateRasterizerStates()
			{
				HRESULT hr;
				D3D11_RASTERIZER_DESC rasterizerDesc{};
				rasterizerDesc.AntialiasedLineEnable = m_settings.antiAliasing;
				rasterizerDesc.CullMode = D3D11_CULL_BACK;
				rasterizerDesc.DepthBias = 0;
				rasterizerDesc.DepthBiasClamp = 0.0f;
				rasterizerDesc.DepthClipEnable = true;
				rasterizerDesc.FrontCounterClockwise = false;
				rasterizerDesc.MultisampleEnable = m_settings.multiSamplingEnable;
				rasterizerDesc.ScissorEnable = false;
				rasterizerDesc.SlopeScaledDepthBias = 0.0f;

				rasterizerDesc.FillMode = D3D11_FILL_SOLID;
				hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerSolid);
				if (FAILED(hr))
					throw std::exception("Failed to create rasterizer state");

				rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
				hr = m_device->CreateRasterizerState(&rasterizerDesc, &m_rasterizerWireframe);
				if (FAILED(hr))
					throw std::exception("Failed to create rasterizer state");

				m_context->RSSetState(m_rasterizerSolid);
			}
			
			LRESULT Graphics_DX11::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
			{
				return m_input.HandleMessage(hwnd, msg, wparam, lparam);
			}
			void Graphics_DX11::Run()
			{
				try
				{
					MSG msg{};
					if (m_startFunction)
						m_startFunction(*this);
					decltype(std::chrono::steady_clock::now()) prevTime = std::chrono::steady_clock::now();
					while (msg.message != WM_QUIT)
					{
						if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
						{
							TranslateMessage(&msg);
							DispatchMessage(&msg);
						}
						else
						{
							if (m_updateFunction)
							{
								decltype(prevTime) currentTime = std::chrono::steady_clock::now();
								m_updateFunction(*this, std::chrono::duration<float>(currentTime - prevTime).count());
								prevTime = currentTime;
							}
						}
					}
				}
				catch (std::exception e)
				{
					MessageBoxA(m_hwnd, e.what(), "Error", MB_OK | MB_ICONERROR);
				}
			}
			Graphics_DX11::P Graphics_DX11::CreateP(GraphicsSettings& settings)
			{
				Graphics_DX11::P gfx = std::make_shared<Graphics_DX11>();
				gfx->Init(settings);
				return gfx;
			}
			Graphics_DX11::U Graphics_DX11::CreateU(GraphicsSettings& settings)
			{
				Graphics_DX11::U gfx = std::make_unique<Graphics_DX11>();
				gfx->Init(settings);
				return gfx;
			}
			void Graphics_DX11::SetScreenAsRenderTarget()
			{
				m_context->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
				m_context->RSSetViewports(1, &m_viewPort);
			}
			void Graphics_DX11::ClearRenderTarget()
			{
				float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
				ClearRenderTarget(clearColor);
			}
			void Graphics_DX11::ClearRenderTarget(float r, float g, float b, float a)
			{
				float clearColor[] = { r, g, b, a };
				ClearRenderTarget(clearColor);
			}
			void Graphics_DX11::ClearRenderTarget(float *color)
			{
				m_context->ClearRenderTargetView(m_renderTargetView, color);
				m_context->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
			}
			void Graphics_DX11::Present()
			{
				m_swapChain->Present(m_settings.vsyncEnable ? 1 : 0, 0);
			}
			void Graphics_DX11::SetPrimitiveTopology_Points()
			{
				m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			}
			void Graphics_DX11::SetPrimitiveTopology_Lines()
			{
				m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			}
			void Graphics_DX11::SetPrimitiveTopology_Triangles()
			{
				m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			}
			void Graphics_DX11::SetFillMode_Solid()
			{
				m_context->RSSetState(m_rasterizerSolid);
			}
			void Graphics_DX11::SetFillMode_Wireframe()
			{
				m_context->RSSetState(m_rasterizerWireframe);
			}
			void Graphics_DX11::SetWindowTitle(const wchar_t *title)
			{
				SetWindowText(m_hwnd, title);
			}
			void Graphics_DX11::Quit()
			{
				PostQuitMessage(0);
			}
			Input& Graphics_DX11::getInput()
			{
				return m_input;
			}
		}
	}
}