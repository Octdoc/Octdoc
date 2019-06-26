#pragma once

#include "wicimageloader.h"

#pragma comment (lib, "windowscodecs.lib")

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			void WICImageLoader::CreateFactory()
			{
				HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)& m_factory);
				if (FAILED(hr))
					throw std::exception("Failed to create imaging factory");
			}
			void WICImageLoader::CreateDecoder()
			{
				HRESULT hr = m_factory->CreateDecoderFromFilename(m_filename.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &m_decoder);
				if (FAILED(hr))
					throw std::exception(("Failed to create image decoder for image: " + hlp::ToString(m_filename.c_str())).c_str());
			}
			void WICImageLoader::GetFrameCount()
			{
				HRESULT hr = m_decoder->GetFrameCount(&m_frameCount);
				if (FAILED(hr) || m_frameCount == 0)
					throw std::exception("Failed to get frame count");
				if (m_frameCount == 1)
				{
					m_animated = false;
					m_hasMips = false;
				}
			}
			COM_Ptr<IWICBitmapFrameDecode> WICImageLoader::LoadFrame(unsigned index)
			{
				COM_Ptr<IWICBitmapFrameDecode> frame;
				HRESULT hr = m_decoder->GetFrame(index, &frame);
				if (FAILED(hr))
					throw std::exception("Failed to get image from decoder");
				return frame;
			}
			COM_Ptr<IWICFormatConverter> WICImageLoader::GetConverter(IWICBitmapFrameDecode* frame)
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
			COM_Ptr<IWICMetadataQueryReader> WICImageLoader::GetMetadataReader(IWICBitmapFrameDecode* frame)
			{
				COM_Ptr<IWICMetadataQueryReader> metadataReader;
				HRESULT hr = frame->GetMetadataQueryReader(&metadataReader);
				//if (FAILED(hr)) throw std::exception("Failed to get metadata reader");	//return nullptr instead
				return metadataReader;
			}
			void WICImageLoader::SetSize(IWICFormatConverter* converter)
			{
				HRESULT hr = converter->GetSize(&m_width, &m_height);
				if (FAILED(hr))
					throw std::exception("Failed to get image size");
				m_frames.resize(m_frameCount);
			}
			bool WICImageLoader::ReadMetadata(IWICMetadataQueryReader* metaDataReader, const wchar_t* name, unsigned& data)
			{
				if (!metaDataReader)
					return false;
				PROPVARIANT propValue;
				PropVariantInit(&propValue);
				HRESULT hr = metaDataReader->GetMetadataByName(name, &propValue);
				if (FAILED(hr))
					return false;
				hr = (propValue.vt == VT_UI2 ? S_OK : E_FAIL);
				if (FAILED(hr))
					return false;
				data = propValue.uiVal;
				PropVariantClear(&propValue);
				return true;
			}
			void WICImageLoader::ReadFirstFrame(IWICBitmapFrameDecode* frame, IWICFormatConverter* converter)
			{
				m_frames[0].frame.resize(m_width * m_height * 4);
				HRESULT hr = converter->CopyPixels(nullptr, m_width * 4, m_frames[0].frame.size(), m_frames[0].frame.data());
				if (FAILED(hr))
					throw std::exception("Failed to copy pixel data");
				if (m_animated)
				{
					unsigned delay = 0;
					auto metadata = GetMetadataReader(frame);
					ReadMetadata(metadata, L"/grctlext/Delay", delay);
					m_frames[0].time = max(float(delay) * 0.01f, 0.09f);
				}
			}
			void WICImageLoader::ReadFrameAnimated(unsigned index)
			{
				auto frame = LoadFrame(index);
				auto converter = GetConverter(frame);
				auto metadata = GetMetadataReader(frame);
				unsigned left = 0, top = 0, width = m_width, height = m_height, delay = 0;
				ReadMetadata(metadata, L"/imgdesc/Left", left);
				ReadMetadata(metadata, L"/imgdesc/Top", top);
				ReadMetadata(metadata, L"/imgdesc/Width", width);
				ReadMetadata(metadata, L"/imgdesc/Height", height);
				ReadMetadata(metadata, L"/grctlext/Delay", delay);
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
			void WICImageLoader::ReadFramesMips(unsigned index)
			{
				auto frame = LoadFrame(index);
				auto converter = GetConverter(frame);
				auto metadata = GetMetadataReader(frame);
				unsigned width = max(m_width >> index, 1);
				unsigned height = max(m_height >> index, 1);
				m_frames[index].frame.resize(width * height * 4);
				HRESULT hr = converter->CopyPixels(nullptr, 4 * width, m_frames[index].frame.size(), m_frames[index].frame.data());
				if (FAILED(hr))
					throw std::exception("Failed to copy pixel data");
			}

			WICImageLoader::WICImageLoader(const wchar_t* filename) :m_filename(filename)
			{
				if (hlp::GetFileExtension(filename) == L".gif")
				{//check frame count later if this really is the case
					m_animated = true;
					m_hasMips = false;
				}
				else
				{
					m_animated = false;
					m_hasMips = true;
				}
				CreateFactory();
				CreateDecoder();
				GetFrameCount();
				auto frame = LoadFrame(0);
				auto converter = GetConverter(frame);
				SetSize(converter);
				ReadFirstFrame(frame, converter);
				if (m_animated)
				{
					for (unsigned i = 1; i < m_frameCount; i++)
						ReadFrameAnimated(i);
				}
				else if (m_hasMips)
				{
					for (unsigned i = 1; i < m_frameCount; i++)
						ReadFramesMips(i);
				}
			}
			ImageCreatorStruct WICImageLoader::ReadyDataForMipmapTextureCreation()
			{
				ImageCreatorStruct imageCreator;
				imageCreator.imageData.resize(m_frameCount);
				for (unsigned i = 0; i < m_frameCount; i++)
				{
					imageCreator.imageData[i].pSysMem = m_frames[i].frame.data();
					imageCreator.imageData[i].SysMemPitch = 4 * max(m_width >> i, 1);
					imageCreator.imageData[i].SysMemSlicePitch = 0;
				}
				imageCreator.mipLevels = m_frameCount;
				imageCreator.generateMips = false;
				return imageCreator;
			}
		};
	}
}