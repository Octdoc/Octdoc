#pragma once

#include "texture.h"

namespace octdoc
{
	namespace gfx
	{
		class RenderTargetTexture :public Texture
		{
			SMART_PTR(RenderTargetTexture)
		private:

		public:
			static RenderTargetTexture::P CreateP(Graphics& graphics, int width, int height);
			static RenderTargetTexture::U CreateU(Graphics& graphics, int width, int height);

			virtual void SetAsRenderTarget(Graphics& graphics) = 0;
		};
	}
}