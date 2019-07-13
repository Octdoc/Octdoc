#pragma once

#include "graphics.h"
#include <vector>

namespace octdoc
{
	namespace gfx
	{
		class Texture
		{
			SMART_PTR(Texture)

		protected:
			unsigned m_width;
			unsigned m_height;

		public:
			Texture();
			virtual ~Texture();
			static Texture::P CreateP(Graphics& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels = 0);
			static Texture::U CreateU(Graphics& graphics, void* data, unsigned width, unsigned height, unsigned mipLevels = 0);
			static Texture::P CreateP(Graphics& graphics, const wchar_t* filename, unsigned mipLevels = 0);
			static Texture::U CreateU(Graphics& graphics, const wchar_t* filename, unsigned mipLevels = 0);

			virtual void SetToRender(Graphics& graphics, unsigned index = 0) = 0;

			inline unsigned getWidth() { return m_width; }
			inline unsigned getHeight() { return m_height; }
		};
	}
}