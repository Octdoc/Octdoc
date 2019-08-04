#pragma once

#include "graphics_dx11.h"
#include <fstream>

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			COM_Ptr<ID3DBlob> LoadShaderCode(LPCWSTR filename);
			COM_Ptr<ID3DBlob> CompileShader(const std::string& shaderCode, const char* entry, const char* target, const wchar_t* errorOutputFile);
		}
	}
}