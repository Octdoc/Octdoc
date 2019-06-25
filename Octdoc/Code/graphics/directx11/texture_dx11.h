#pragma once

#include "graphics/texture.h"
#include "graphics_dx11.h"
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
				COM_Ptr<ID3D11ShaderResourceView> m_shaderResourceView;

			private:
				void CreateTexture(Graphics_DX11& graphics, void* data, int width, int height);
				void LoadTexture(Graphics_DX11& graphics, const wchar_t* filename);
				void LoadTarga(Graphics_DX11& graphics, const wchar_t* filename);

			public:
				Texture_DX11(Graphics_DX11& graphics, void* data, int width, int height);
				Texture_DX11(Graphics_DX11& graphics, const wchar_t* filename);
				~Texture_DX11();
				static Texture_DX11::P CreateP(Graphics_DX11& graphics, void* data, int width, int height);
				static Texture_DX11::U CreateU(Graphics_DX11& graphics, void* data, int width, int height);
				static Texture_DX11::P CreateP(Graphics_DX11& graphics, const wchar_t* filename);
				static Texture_DX11::U CreateU(Graphics_DX11& graphics, const wchar_t* filename);

				virtual void SetToRender(Graphics& graphics, unsigned index = 0) override;
			};
		}
	}
}