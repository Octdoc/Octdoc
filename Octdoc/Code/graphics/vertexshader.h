#pragma once

#include "graphics.h"

namespace octdoc
{
	namespace gfx
	{
		class VertexShader
		{
			SMART_PTR(VertexShader)
		protected:
			unsigned m_modelType;

		public:
			virtual ~VertexShader();
			static VertexShader::P CreateP(Graphics& graphics, const wchar_t* shaderFileName, unsigned modelType);
			static VertexShader::U CreateU(Graphics& graphics, const wchar_t* shaderFileName, unsigned modelType);
			static VertexShader::P CreateP(Graphics& graphics, unsigned modelType);
			static VertexShader::U CreateU(Graphics& graphics, unsigned modelType);
			static VertexShader::P CreatePFromString(Graphics& graphics, const char* shaderCode, unsigned modelType);
			static VertexShader::U CreateUFromString(Graphics& graphics, const char* shaderCode, unsigned modelType);

			virtual void SetShaderToRender(Graphics& graphics) = 0;
		};
	}
}