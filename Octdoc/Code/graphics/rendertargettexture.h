#pragma once

#include "texture.h"

namespace octdoc
{
	namespace gfx
	{
		class RenderTargetTexture :public Texture
		{
			SMART_PTR(RenderTargetTexture)
		protected:
			virtual void ClearRenderTarget(Graphics& graphics, float clearColor[4]) = 0;

		public:
			static RenderTargetTexture::P CreateP(Graphics& graphics, int width, int height);
			static RenderTargetTexture::U CreateU(Graphics& graphics, int width, int height);

			void ClearRenderTarget(Graphics& graphics);
			void ClearRenderTarget(Graphics& graphics, float r, float g, float b, float a = 1.0f);
			virtual void SetAsRenderTarget(Graphics& graphics) = 0;
		};
	}
}