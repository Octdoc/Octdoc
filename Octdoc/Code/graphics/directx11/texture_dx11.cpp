#include "texture_dx11.h"
#include "helpers/fileloaders.h"
#include "C://Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/D3DX11tex.h"

#pragma comment (lib, "C://Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/D3Dx11.lib")

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			std::map<std::wstring, Texture_DX11::W> Texture_DX11::m_loadedTextures;
			void Texture_DX11::ClearReleasedTextures()
			{
				for (auto tex : m_loadedTextures)
				{
					if (tex.second.expired())
					{
						m_loadedTextures.erase(tex.first);
						return;
					}
				}
			}
			void Texture_DX11::CreateTexture(Graphics_DX11& graphics, void* data, int width, int height)
			{
				ID3D11Device* device = graphics.getDevice();
				ID3D11DeviceContext* context = graphics.getContext();
				HRESULT hr;
				COM_Ptr<ID3D11Texture2D> texture;
				D3D11_TEXTURE2D_DESC t2dd{};
				D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
				D3D11_SUBRESOURCE_DATA srd{};

				t2dd.Width = width;
				t2dd.Height = height;
				t2dd.MipLevels = 1;
				t2dd.ArraySize = 1;
				t2dd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				t2dd.SampleDesc.Count = 1;
				t2dd.SampleDesc.Quality = 0;
				t2dd.Usage = D3D11_USAGE_IMMUTABLE;
				t2dd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				t2dd.CPUAccessFlags = 0;
				t2dd.MiscFlags = 0;

				srd.pSysMem = data;
				srd.SysMemPitch = width * 4;
				srd.SysMemSlicePitch = 0;
				hr = device->CreateTexture2D(&t2dd, &srd, &texture);
				if (FAILED(hr))
				{
					auto error = GetLastError();
					throw std::exception("Failed to create texture");
				}

				srvd.Format = t2dd.Format;
				srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvd.Texture2D.MostDetailedMip = 0;
				srvd.Texture2D.MipLevels = -1;

				hr = device->CreateShaderResourceView(texture, &srvd, &m_shaderResourceView);
				if (FAILED(hr))
					throw std::exception("Failed to create shader resource view");
			}
			void Texture_DX11::LoadTexture(Graphics_DX11& graphics, const wchar_t* filename)
			{
				std::wstring extension = hlp::GetFileExtension(filename);
				if (extension == L".tga")
					LoadTarga(graphics, filename);
				else
				{
					HRESULT hr;
					hr = D3DX11CreateShaderResourceViewFromFile(graphics.getDevice(), filename, nullptr, nullptr, &m_shaderResourceView, nullptr);
					if (FAILED(hr))
						throw std::exception((std::string("Failed to create texture: ") + hlp::ToString(filename)).c_str());
				}
			}
			void Texture_DX11::LoadTarga(Graphics_DX11& graphics, const wchar_t* filename)
			{
				std::vector<unsigned char> pixels;
				int width, height;
				hlp::LoadTargaFromFile(filename, pixels, width, height);
				CreateTexture(graphics, pixels.data(), width, height);
			}
			Texture_DX11::Texture_DX11(Graphics_DX11& graphics, void* data, int width, int height)
			{
				CreateTexture(graphics, data, width, height);
			}
			Texture_DX11::Texture_DX11(Graphics_DX11& graphics, const wchar_t *filename)
			{
				LoadTexture(graphics, filename);
			}
			Texture_DX11::~Texture_DX11()
			{
				ClearReleasedTextures();
			}
			Texture_DX11::P Texture_DX11::CreateP(Graphics_DX11& graphics, void* data, int width, int height)
			{
				return std::make_shared<Texture_DX11>(graphics, data, width, height);
			}
			Texture_DX11::U Texture_DX11::CreateU(Graphics_DX11& graphics, void* data, int width, int height)
			{
				return std::make_unique<Texture_DX11>(graphics, data, width, height);
			}
			Texture_DX11::P Texture_DX11::CreateP(Graphics_DX11& graphics, const wchar_t *filename)
			{
				auto tex = m_loadedTextures.find(filename);
				if (tex != m_loadedTextures.end())
					return tex->second.lock();
				Texture_DX11::P texture = std::make_shared<Texture_DX11>(graphics, filename);
				m_loadedTextures[filename] = texture;
				return texture;
			}
			Texture_DX11::U Texture_DX11::CreateU(Graphics_DX11& graphics, const wchar_t *filename)
			{
				return std::make_unique<Texture_DX11>(graphics, filename);
			}
			void Texture_DX11::SetToRender(Graphics& graphics, unsigned index)
			{
				ID3D11DeviceContext *context = ((Graphics_DX11&)graphics).getContext();
				context->PSSetShaderResources(index, 1, &m_shaderResourceView);
			}
		}
	}
}