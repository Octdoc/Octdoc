#pragma once

#include "graphics/texture.h"
#include "wicimageloader.h"
#include <chrono>
#include <map>
#include <string>


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
				struct Frame
				{
					COM_Ptr<ID3D11ShaderResourceView> shaderResourceView;
					float frameTime;

					Frame();
				};
				std::vector<Frame> m_frames;
				decltype(std::chrono::steady_clock::now()) m_prevTime;
				unsigned m_currentFrameIndex;
				float m_frameTimeLeft;	//how much time till the next frame comes
								
			private:
				COM_Ptr<ID3D11ShaderResourceView> CreateTexture(Graphics_DX11& graphics, void* data, unsigned mipLevels);
				COM_Ptr<ID3D11ShaderResourceView> CreateTexture(Graphics_DX11& graphics, ImageCreatorStruct& imageCreator);
				void LoadTexture(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels);
				void LoadTarga(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels);
				void LoadWithWIC(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels);
				void InitData();

			public:
				Texture_DX11(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels = 0);
				Texture_DX11(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels = 0);
				virtual ~Texture_DX11() override;
				static Texture_DX11::P CreateP(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels = 0);
				static Texture_DX11::U CreateU(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels = 0);
				static Texture_DX11::P CreateP(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels = 0);
				static Texture_DX11::U CreateU(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels = 0);

				virtual void SetToRender(Graphics& graphics, unsigned index = 0) override;
			};
		}
	}
}