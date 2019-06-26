#include "texture_dx11.h"
#include "helpers/fileloaders.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			Texture_DX11::Frame::Frame() :shaderResourceView(), frameTime(0.0f) {}
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
			COM_Ptr<ID3D11ShaderResourceView> Texture_DX11::CreateTexture(Graphics_DX11& graphics, void* data, unsigned mipLevels)
			{
				D3D11_SUBRESOURCE_DATA imgData{};
				imgData.pSysMem = data;
				imgData.SysMemPitch = m_width * 4;
				imgData.SysMemSlicePitch = 0;
				ImageCreatorStruct imageCreator;
				imageCreator.imageData.push_back(imgData);
				imageCreator.mipLevels = mipLevels;
				imageCreator.generateMips = mipLevels != 1;
				return CreateTexture(graphics, imageCreator);
			}
			COM_Ptr<ID3D11ShaderResourceView> Texture_DX11::CreateTexture(Graphics_DX11& graphics, ImageCreatorStruct& imageCreator)
			{
				COM_Ptr<ID3D11ShaderResourceView> shaderResourceView;
				ID3D11Device* device = graphics.getDevice();
				ID3D11DeviceContext* context = graphics.getContext();
				HRESULT hr;
				COM_Ptr<ID3D11Texture2D> texture;

				D3D11_TEXTURE2D_DESC t2dd{};
				t2dd.Width = m_width;
				t2dd.Height = m_height;
				t2dd.MipLevels = imageCreator.mipLevels;
				t2dd.ArraySize = 1;
				t2dd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				t2dd.SampleDesc.Count = 1;
				t2dd.SampleDesc.Quality = 0;
				t2dd.CPUAccessFlags = 0;
				if (imageCreator.generateMips)
				{
					t2dd.Usage = D3D11_USAGE_DEFAULT;
					t2dd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
					t2dd.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
				}
				else
				{
					t2dd.Usage = D3D11_USAGE_IMMUTABLE;
					t2dd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
					t2dd.MiscFlags = 0;
				}

				hr = device->CreateTexture2D(&t2dd, imageCreator.generateMips ? nullptr : &imageCreator.imageData[0], &texture);
				if (FAILED(hr))
					throw std::exception("Failed to create texture");
				if (imageCreator.generateMips)
					context->UpdateSubresource(texture, 0, nullptr, imageCreator.imageData[0].pSysMem, imageCreator.imageData[0].SysMemPitch, imageCreator.imageData[0].SysMemSlicePitch);

				D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
				srvd.Format = t2dd.Format;
				srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvd.Texture2D.MostDetailedMip = 0;
				srvd.Texture2D.MipLevels = imageCreator.generateMips ? -1 : imageCreator.mipLevels;
				hr = device->CreateShaderResourceView(texture, &srvd, &shaderResourceView);
				if (FAILED(hr))
					throw std::exception("Failed to create shader resource view");
				if (imageCreator.generateMips)
					context->GenerateMips(shaderResourceView);

				return shaderResourceView;
			}
			void Texture_DX11::LoadTexture(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				std::wstring extension = hlp::GetFileExtension(filename);
				if (extension == L".tga")
					LoadTarga(graphics, filename, mipLevels);
				else
					LoadWithWIC(graphics, filename, mipLevels);
			}
			void Texture_DX11::LoadTarga(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				std::vector<unsigned char> pixels;
				hlp::LoadTargaFromFile(filename, pixels, m_width, m_height);
				m_frames.push_back(Frame());
				m_frames[0].shaderResourceView = CreateTexture(graphics, pixels.data(), mipLevels);
			}
			void Texture_DX11::LoadWithWIC(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				WICImageLoader loader(filename);
				unsigned frameCount = loader.isAnimated() ? loader.getFrameCount() : 1;
				m_width = loader.getWidth();
				m_height = loader.getHeight();
				m_frames.resize(frameCount);
				for (unsigned i = 0; i < frameCount; i++)
				{
					if (loader.hasMips())
					{
						ImageCreatorStruct imageCreator = loader.ReadyDataForMipmapTextureCreation();
						m_frames[i].shaderResourceView = CreateTexture(graphics, imageCreator);
					}
					else
						m_frames[i].shaderResourceView = CreateTexture(graphics, loader.getFrame(i).frame.data(), mipLevels);
					m_frames[i].frameTime = loader.getFrame(i).time;
				}
				m_prevTime = std::chrono::steady_clock::now();
			}
			void Texture_DX11::InitData()
			{
				m_currentFrameIndex = 0;
				m_frameTimeLeft = 0.0f;
				m_prevTime = decltype(m_prevTime)();
			}
			Texture_DX11::Texture_DX11(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels)
			{
				InitData();
				m_width = width;
				m_height = height;
				m_frames.push_back(Frame());
				m_frames[0].shaderResourceView = CreateTexture(graphics, data, mipLevels);
			}
			Texture_DX11::Texture_DX11(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				InitData();
				LoadTexture(graphics, filename, mipLevels);
			}
			Texture_DX11::~Texture_DX11()
			{
				ClearReleasedTextures();
			}
			Texture_DX11::P Texture_DX11::CreateP(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels)
			{
				return std::make_shared<Texture_DX11>(graphics, data, width, height, mipLevels);
			}
			Texture_DX11::U Texture_DX11::CreateU(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels)
			{
				return std::make_unique<Texture_DX11>(graphics, data, width, height, mipLevels);
			}
			Texture_DX11::P Texture_DX11::CreateP(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				auto tex = m_loadedTextures.find(filename);
				if (tex != m_loadedTextures.end())
					return tex->second.lock();
				Texture_DX11::P texture = std::make_shared<Texture_DX11>(graphics, filename, mipLevels);
				m_loadedTextures[filename] = texture;
				return texture;
			}
			Texture_DX11::U Texture_DX11::CreateU(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				return std::make_unique<Texture_DX11>(graphics, filename, mipLevels);
			}
			void Texture_DX11::SetToRender(Graphics& graphics, unsigned index)
			{
				if (m_frames.empty())
					return;
				ID3D11DeviceContext* context = ((Graphics_DX11&)graphics).getContext();
				if (m_frames.size() > 1)
				{
					auto currTime = std::chrono::steady_clock::now();
					float delta = std::chrono::duration<float>(currTime - m_prevTime).count();
					m_prevTime = currTime;

					while (delta > m_frameTimeLeft)
					{
						delta -= m_frameTimeLeft;
						m_currentFrameIndex = (m_currentFrameIndex + 1) % m_frames.size();
						m_frameTimeLeft = m_frames[m_currentFrameIndex].frameTime;
					}
					m_frameTimeLeft -= delta;
					context->PSSetShaderResources(index, 1, &m_frames[m_currentFrameIndex].shaderResourceView);
				}
				else
				{
					context->PSSetShaderResources(index, 1, &m_frames[0].shaderResourceView);
				}
			}
		}
	}
}