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
			virtual ~PixelShader();
			static PixelShader::P CreateP(Graphics& graphics, const wchar_t* shaderFileName);
			static PixelShader::U CreateU(Graphics& graphics, const wchar_t* shaderFileName);
			static PixelShader::P CreateP(Graphics& graphics, unsigned modelType);
			static PixelShader::U CreateU(Graphics& graphics, unsigned modelType);
			static PixelShader::P CreatePFromString(Graphics& graphics, const char* shaderCode);
			static PixelShader::U CreateUFromString(Graphics& graphics, const char* shaderCode);

			virtual void SetShaderToRender(Graphics& graphics) = 0;
		};
	}
}