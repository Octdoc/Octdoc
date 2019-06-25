#pragma once

#include "graphics/texture.h"
#include "graphics_dx11.h"
#include <map>
#include <string>
#include <chrono>

//#define DIRECTX_SDK
/*
Loading 1920x1200 .jpg image 1000 times with SDK function vs WIC
Disc speed does not make noticable difference
SDK -> 38.47s
WIC -> 27.32s (27.9s with generated mips)
*/

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			class Texture_DX11 :public Texture
			{
				SMART_PTR(Texture)
			private:
				static std::map<std::wstring, Texture_DX11::W> m_loadedTextures;
				static void ClearReleasedTextures();

			private:
				COM_Ptr<ID3D11ShaderResourceView> m_shaderResourceView;

				struct Frame
				{
					COM_Ptr<ID3D11ShaderResourceView> shaderResourceView;
					float frameTime;
				};
				std::vector<Frame> m_frames;
				decltype(std::chrono::steady_clock::now()) m_prevTime;
				unsigned m_currentFrameIndex;
				float m_frameTimeLeft;	//how much time till the next frame comes
				bool m_animated;
								
			private:
				void CreateTexture(Graphics_DX11& graphics, void* data, unsigned mipLevels, COM_Ptr<ID3D11ShaderResourceView>& shaderResourceView);
				void LoadTexture(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels);
				void LoadTarga(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels);
#ifdef DIRECTX_SDK
				void LoadWithSDK(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels);
#else
				void LoadWithWIC(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels);
#endif
				void InitData(bool animated);

			public:
				Texture_DX11(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels = 0);
				Texture_DX11(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels = 0);
				~Texture_DX11();
				static Texture_DX11::P CreateP(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels = 0);
				static Texture_DX11::U CreateU(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels = 0);
				static Texture_DX11::P CreateP(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels = 0);
				static Texture_DX11::U CreateU(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels = 0);

				virtual void SetToRender(Graphics& graphics, unsigned index = 0) override;
			};
		}
	}
}