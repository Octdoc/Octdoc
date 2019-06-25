#pragma once

#include "graphics.h"

namespace octdoc
{
	namespace gfx
	{
		class PixelShader
		{
			SMART_PTR(PixelShader)

		public:
			static PixelShader::P CreateP(Graphics& graphics, const wchar_t* shaderFileName);
			static PixelShader::U CreateU(Graphics& graphics, const wchar_t* shaderFileName);
			static PixelShader::P CreateP(Graphics& graphics, unsigned modelType);
			static PixelShader::U CreateU(Graphics& graphics, unsigned modelType);

			virtual void SetShaderToRender(Graphics& graphics) = 0;
		};
	}
}