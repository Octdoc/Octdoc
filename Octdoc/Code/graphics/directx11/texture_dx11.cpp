#include "texture_dx11.h"
#include "helpers/fileloaders.h"

#ifdef DIRECTX_SDK
#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/D3DX11tex.h"
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x64/D3Dx11.lib")
#else
#include<wincodec.h>
#pragma comment (lib, "windowscodecs.lib")
#endif


namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
#ifndef DIRECTX_SDK
			class WICImageLoader
			{
				struct Frame
				{
					std::vector<unsigned char> frame;
					float time;
				};
				std::wstring m_filename;
				COM_Ptr<IWICImagingFactory> m_factory;
				COM_Ptr<IWICBitmapDecoder> m_decoder;
				unsigned m_frameCount;
				std::vector<Frame> m_frames;
				unsigned m_width;
				unsigned m_height;

			private:
				void CreateFactory()
				{
					HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)& m_factory);
					if (FAILED(hr))
						throw std::exception("Failed to create imaging factory");
				}
				void CreateDecoder()
				{
					HRESULT hr = m_factory->CreateDecoderFromFilename(m_filename.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &m_decoder);
					if (FAILED(hr))
						throw std::exception(("Failed to create image decoder for image: " + hlp::ToString(m_filename.c_str())).c_str());
				}
				void GetFrameCount()
				{
					HRESULT hr = m_decoder->GetFrameCount(&m_frameCount);
					if (FAILED(hr) || m_frameCount == 0)
						throw std::exception("Failed to get frame count");
				}
				COM_Ptr<IWICBitmapFrameDecode> LoadFrame(unsigned index)
				{
					COM_Ptr<IWICBitmapFrameDecode> frame;
					HRESULT hr = m_decoder->GetFrame(index, &frame);
					if (FAILED(hr))
						throw std::exception("Failed to get image from decoder");
					return frame;
				}
				COM_Ptr<IWICFormatConverter> GetConverter(IWICBitmapFrameDecode* frame)
				{
					COM_Ptr<IWICFormatConverter> converter;
					HRESULT hr = m_factory->CreateFormatConverter(&converter);
					if (FAILED(hr))
						throw std::exception("Failed to create format converter");

					hr = converter->Initialize(frame, GUID_WICPixelFormat32bppPRGBA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);
					if (FAILED(hr))
						throw std::exception("Failed to initialize converter");

					return converter;
				}
				COM_Ptr<IWICMetadataQueryReader> GetMetadataReader(IWICBitmapFrameDecode* frame)
				{
					COM_Ptr<IWICMetadataQueryReader> metadataReader;
					HRESULT hr = frame->GetMetadataQueryReader(&metadataReader);
					if (FAILED(hr))
						throw std::exception("Failed to get metadata reader");
					return metadataReader;
				}
				void SetSize(IWICFormatConverter* converter)
				{
					HRESULT hr = converter->GetSize(&m_width, &m_height);
					if (FAILED(hr))
						throw std::exception("Failed to get image size");
					m_frames.resize(m_frameCount);
					for (auto& frame : m_frames)
						frame.frame.resize(m_width * m_height * 4);
				}
				void ReadFirstFrame(IWICBitmapFrameDecode* frame, IWICFormatConverter* converter)
				{
					HRESULT hr = converter->CopyPixels(nullptr, m_width * 4, m_frames[0].frame.size(), m_frames[0].frame.data());
					if (FAILED(hr))
						throw std::exception("Failed to copy pixel data");
					if (m_frameCount > 1)
					{
						auto metadata = GetMetadataReader(frame);
						unsigned delay = ReadMetadata(metadata, L"/grctlext/Delay");
						m_frames[0].time = max(float(delay) * 0.01f, 0.09f);
					}
				}
				unsigned ReadMetadata(IWICMetadataQueryReader* metaDataReader, const wchar_t* name)
				{
					PROPVARIANT propValue;
					PropVariantInit(&propValue);
					HRESULT hr = metaDataReader->GetMetadataByName(name, &propValue);
					if (FAILED(hr))
						throw std::exception(("Cannot read metadata: " + hlp::ToString(name)).c_str());
					hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
					if (FAILED(hr))
						throw std::exception("Incorrect metadata type");
					unsigned data = propValue.uiVal;
					PropVariantClear(&propValue);
					return data;
				}
				void ReadFrame(unsigned index)
				{
					auto frame = LoadFrame(index);
					auto converter = GetConverter(frame);
					auto metadata = GetMetadataReader(frame);
					unsigned left = ReadMetadata(metadata, L"/imgdesc/Left");
					unsigned top = ReadMetadata(metadata, L"/imgdesc/Top");
					unsigned width = ReadMetadata(metadata, L"/imgdesc/Width");
					unsigned height = ReadMetadata(metadata, L"/imgdesc/Height");
					unsigned delay = ReadMetadata(metadata, L"/grctlext/Delay");
					std::vector<unsigned char> tmpBuffer(width * height * 4);
					HRESULT hr = converter->CopyPixels(nullptr, 4 * width, tmpBuffer.size(), tmpBuffer.data());
					if (FAILED(hr))
						throw std::exception("Failed to copy pixel data");
					m_frames[index].frame = m_frames[index - 1].frame;
					m_frames[index].time = max(float(delay) * 0.01f, 0.09f);
					for (unsigned y = 0; y < height; y++)
					{
						for (unsigned x = 0; x < width; x++)
						{
							unsigned char* dst = m_frames[index].frame.data();
							unsigned char* src = tmpBuffer.data();
							if (src[(x + y * width) * 4 + 3])
							{
								dst[(left + x + (top + y) * m_width) * 4 + 0] = src[(x + y * width) * 4 + 0];
								dst[(left + x + (top + y) * m_width) * 4 + 1] = src[(x + y * width) * 4 + 1];
								dst[(left + x + (top + y) * m_width) * 4 + 2] = src[(x + y * width) * 4 + 2];
								dst[(left + x + (top + y) * m_width) * 4 + 3] = src[(x + y * width) * 4 + 3];
							}
						}
					}
				}

			public:
				WICImageLoader(const wchar_t* filename) :m_filename(filename)
				{
					CreateFactory();
					CreateDecoder();
					GetFrameCount();
					auto frame = LoadFrame(0);
					auto converter = GetConverter(frame);
					SetSize(converter);
					ReadFirstFrame(frame, converter);
					for (unsigned i = 1; i < m_frameCount; i++)
						ReadFrame(i);
				}
				inline unsigned getFrameCount() { return  m_frameCount; }
				inline Frame& getFrame(unsigned index) { return  m_frames[index]; }
				inline unsigned getWidth() { return m_width; }
				inline unsigned getHeight() { return m_height; }
			};
#endif
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
			void Texture_DX11::CreateTexture(Graphics_DX11& graphics, void* data, unsigned mipLevels, COM_Ptr<ID3D11ShaderResourceView>& shaderResourceView)
			{
				ID3D11Device* device = graphics.getDevice();
				ID3D11DeviceContext* context = graphics.getContext();
				HRESULT hr;
				COM_Ptr<ID3D11Texture2D> texture;

				D3D11_TEXTURE2D_DESC t2dd{};
				t2dd.Width = m_width;
				t2dd.Height = m_height;
				t2dd.MipLevels = mipLevels;
				t2dd.ArraySize = 1;
				t2dd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				t2dd.SampleDesc.Count = 1;
				t2dd.SampleDesc.Quality = 0;
				t2dd.Usage = D3D11_USAGE_DEFAULT;
				t2dd.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
				t2dd.CPUAccessFlags = 0;
				t2dd.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

				D3D11_SUBRESOURCE_DATA srd{};
				srd.pSysMem = data;
				srd.SysMemPitch = m_width * 4;
				srd.SysMemSlicePitch = 0;
				hr = device->CreateTexture2D(&t2dd, mipLevels == 1 ? &srd : nullptr, &texture);
				if (FAILED(hr))
					throw std::exception("Failed to create texture");
				if (mipLevels != 1)
					context->UpdateSubresource(texture, 0, nullptr, srd.pSysMem, srd.SysMemPitch, srd.SysMemSlicePitch);

				D3D11_SHADER_RESOURCE_VIEW_DESC srvd{};
				srvd.Format = t2dd.Format;
				srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvd.Texture2D.MostDetailedMip = 0;
				srvd.Texture2D.MipLevels = -1;
				hr = device->CreateShaderResourceView(texture, &srvd, &shaderResourceView);
				if (FAILED(hr))
					throw std::exception("Failed to create shader resource view");
				if (mipLevels != 1)
					context->GenerateMips(shaderResourceView);
			}
			void Texture_DX11::LoadTexture(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				std::wstring extension = hlp::GetFileExtension(filename);
				if (extension == L".tga")
					LoadTarga(graphics, filename, mipLevels);
				else
#ifdef DIRECTX_SDK
					LoadWithSDK(graphics, filename, mipLevels);
#else
					LoadWithWIC(graphics, filename, mipLevels);
#endif
			}
			void Texture_DX11::LoadTarga(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				std::vector<unsigned char> pixels;
				hlp::LoadTargaFromFile(filename, pixels, m_width, m_height);
				CreateTexture(graphics, pixels.data(), mipLevels, m_shaderResourceView);
				InitData(false);
			}
#ifdef DIRECTX_SDK
			void Texture_DX11::LoadWithSDK(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				HRESULT hr;
				hr = D3DX11CreateShaderResourceViewFromFile(graphics.getDevice(), filename, nullptr, nullptr, &m_shaderResourceView, nullptr);
				if (FAILED(hr))
					throw std::exception((std::string("Failed to create texture: ") + hlp::ToString(filename)).c_str());

				COM_Ptr<ID3D11Texture2D> texture;
				COM_Ptr<ID3D11Resource> resource;
				m_shaderResourceView->GetResource(&resource);
				resource->QueryInterface<ID3D11Texture2D>(&texture);
				D3D11_TEXTURE2D_DESC texDesc;
				texture->GetDesc(&texDesc);
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				m_shaderResourceView->GetDesc(&srvDesc);
				m_width = texDesc.Width;
				m_height = texDesc.Height;
				InitData(false);
			}
#else
			void Texture_DX11::LoadWithWIC(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
				WICImageLoader loader(filename);
				unsigned frameCount = loader.getFrameCount();
				m_width = loader.getWidth();
				m_height = loader.getHeight();
				if (frameCount > 1)
				{
					m_frames.resize(frameCount);
					for (unsigned i = 0; i < frameCount; i++)
					{
						CreateTexture(graphics, loader.getFrame(i).frame.data(), mipLevels, m_frames[i].shaderResourceView);
						m_shaderResourceView.Release();
						m_frames[i].frameTime = loader.getFrame(i).time;
					}
					InitData(true);
				}
				else
				{
					CreateTexture(graphics, loader.getFrame(0).frame.data(), mipLevels, m_shaderResourceView);
					InitData(false);
				}
			}
			void Texture_DX11::InitData(bool animated)
			{
				if (animated)
				{
					m_shaderResourceView.Release();
					m_prevTime = std::chrono::steady_clock::now();
					m_currentFrameIndex = 0;
					m_frameTimeLeft = m_frames[0].frameTime;
					m_animated = true;
				}
				else
				{
					m_frames.clear();
					m_prevTime = decltype(m_prevTime)();
					m_currentFrameIndex = 0;
					m_frameTimeLeft = 0.0f;
					m_animated = false;
				}
			}
#endif
			Texture_DX11::Texture_DX11(Graphics_DX11& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels)
			{
				m_width = width;
				m_height = height;
				CreateTexture(graphics, data, mipLevels, m_shaderResourceView);
				InitData(false);
			}
			Texture_DX11::Texture_DX11(Graphics_DX11& graphics, const wchar_t* filename, unsigned mipLevels)
			{
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
				ID3D11DeviceContext* context = ((Graphics_DX11&)graphics).getContext();
				if (m_animated)
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
					context->PSSetShaderResources(index, 1, &m_shaderResourceView);
				}
			}
		}
	}
}