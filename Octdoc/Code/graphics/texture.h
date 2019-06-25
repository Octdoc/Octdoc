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

		public:
			static Texture::P CreateP(Graphics& graphics, void* data, int width, int height);
			static Texture::U CreateU(Graphics& graphics, void* data, int width, int height);
			static Texture::P CreateP(Graphics& graphics, const wchar_t* filename);
			static Texture::U CreateU(Graphics& graphics, const wchar_t* filename);

			virtual void SetToRender(Graphics& graphics, unsigned index = 0) = 0;
		};
	}
}