#include "fileloaders.h"
#include <fstream>
#include <exception>

namespace octdoc
{
	namespace hlp
	{
		void LoadTargaFromFile(const wchar_t* filename, std::vector<unsigned char>& pixels, int& width, int& height)
		{
			struct TargaHeader
			{
				uint8 idLength;
				uint8 colormMapType;
				uint8 imageType;
				uint16 colorMapOrigin;
				uint16 colorMapLength;
				uint8 colorMapDepth;
				uint16 xOrigin;
				uint16 yOrigin;
				uint16 width;
				uint16 height;
				uint8 bpp;
				uint8 imageDesc;

				void Read(std::istream& stream)
				{
					stream.read((char*)& idLength, 1);
					stream.read((char*)& colormMapType, 1);
					stream.read((char*)& imageType, 1);
					stream.read((char*)& colorMapOrigin, 2);
					stream.read((char*)& colorMapLength, 2);
					stream.read((char*)& colorMapDepth, 1);
					stream.read((char*)& xOrigin, 2);
					stream.read((char*)& yOrigin, 2);
					stream.read((char*)& width, 2);
					stream.read((char*)& height, 2);
					stream.read((char*)& bpp, 1);
					stream.read((char*)& imageDesc, 1);
				}
			};

			std::ifstream infile(filename, std::ios::in | std::ios::binary);
			if (!infile.is_open())
				throw std::exception((std::string("Failed to open file: ") + ToString(filename)).c_str());

			TargaHeader header;
			header.Read(infile);
			infile.ignore(header.idLength);

			if (header.imageType != 2 && header.imageType != 10)
				throw std::exception((std::string("Failed to load file: ") + ToString(filename)).c_str());
			if (header.bpp != 24 && header.bpp != 32)
				throw std::exception((std::string("Failed to load file: ") + ToString(filename)).c_str());

			int pixelByteCount = header.bpp / 8;
			width = header.width;
			height = header.height;
			pixels.resize(width * height * 4);

			uint8 pixel[4]{ 0x00, 0x00, 0x00, 0xff };
			int pixelOffset = (width * height * 4) - (width * 4);
			if (header.imageType == 2)
			{
				for (int y = 0; y < height; y++)
				{
					for (int x = 0; x < width; x++)
					{
						infile.read((char*)& pixel, pixelByteCount);
						pixels[pixelOffset + 0] = pixel[2];
						pixels[pixelOffset + 1] = pixel[1];
						pixels[pixelOffset + 2] = pixel[0];
						pixels[pixelOffset + 3] = pixel[3];

						pixelOffset += 4;
					}
					pixelOffset -= (width * 8);
				}
			}
			else
			{
				char packetHeader;
				int len, x = 0, y = 0;
				int n = 0;
				while (n < height * width)
				{
					infile.read(&packetHeader, 1);
					len = (packetHeader & 0x7f) + 1;
					if (packetHeader & 0x80)
					{
						infile.read((char*)& pixel, pixelByteCount);
						for (int i = 0; i < len; i++)
						{
							pixels[pixelOffset + 0] = pixel[2];
							pixels[pixelOffset + 1] = pixel[1];
							pixels[pixelOffset + 2] = pixel[0];
							pixels[pixelOffset + 3] = pixel[3];
							pixelOffset += 4;
							if (++x == width)
							{
								x = 0;
								y++;
								pixelOffset -= (width * 8);
							}
						}
					}
					else
					{
						for (int i = 0; i < len; i++)
						{
							infile.read((char*)& pixel, pixelByteCount);
							pixels[pixelOffset + 0] = pixel[2];
							pixels[pixelOffset + 1] = pixel[1];
							pixels[pixelOffset + 2] = pixel[0];
							pixels[pixelOffset + 3] = pixel[3];
							pixelOffset += 4;
							if (++x == width)
							{
								x = 0;
								y++;
								pixelOffset -= (width * 8);
							}
						}
					}
					n += len;
				}
			}
		}
	}
}