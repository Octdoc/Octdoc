#pragma once

#include "comptr.h"
#include "helpers/fileloaders.h"
#include "graphics_dx11.h"
#include <wincodec.h>

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			struct ImageCreatorStruct
			{
				std::vector<D3D11_SUBRESOURCE_DATA> imageData;
				unsigned mipLevels;
				bool generateMips;
			};

			class WICImageLoader
			{
				struct Frame
				{
					std::vector<unsigned char> frame;
					float time;

					Frame() :time(0.1f) {}
				};
				std::wstring m_filename;
				bool m_animated;
				bool m_hasMips;
				COM_Ptr<IWICImagingFactory> m_factory;
				COM_Ptr<IWICBitmapDecoder> m_decoder;
				unsigned m_frameCount;
				std::vector<Frame> m_frames;
				unsigned m_width;
				unsigned m_height;

			private:
				void CreateFactory();
				void CreateDecoder();
				void GetFrameCount();
				COM_Ptr<IWICBitmapFrameDecode> LoadFrame(unsigned index);
				COM_Ptr<IWICFormatConverter> GetConverter(IWICBitmapFrameDecode* frame);
				COM_Ptr<IWICMetadataQueryReader> GetMetadataReader(IWICBitmapFrameDecode* frame);
				void SetSize(IWICFormatConverter* converter);
				bool ReadMetadata(IWICMetadataQueryReader* metaDataReader, const wchar_t* name, unsigned& data);
				void ReadFirstFrame(IWICBitmapFrameDecode* frame, IWICFormatConverter* converter);
				void ReadFrameAnimated(unsigned index);
				void ReadFramesMips(unsigned index);

			public:
				WICImageLoader(const wchar_t* filename);
				ImageCreatorStruct ReadyDataForMipmapTextureCreation();
				inline unsigned getFrameCount() { return  m_frameCount; }
				inline Frame& getFrame(unsigned index) { return  m_frames[index]; }
				inline unsigned getWidth() { return m_width; }
				inline unsigned getHeight() { return m_height; }
				inline bool isAnimated() { return m_animated; }
				inline bool hasMips() { return m_hasMips; }
			};
		}
	}
}