#include "shaderbase_dx11.h"


namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			COM_Ptr<ID3DBlob> LoadShaderCode(LPCWSTR filename)
			{
				HRESULT hr;
				std::fstream infile(filename, std::ios::in | std::ios::binary);
				if (!infile.good())
					return nullptr;
				auto begin = infile.tellg();
				infile.seekg(0, std::ios::end);
				auto end = infile.tellg();
				infile.seekg(0);
				COM_Ptr<ID3DBlob> shaderCode;
				hr = D3DCreateBlob((SIZE_T)(end - begin), &shaderCode);
				if (FAILED(hr))
					return nullptr;
				infile.read((char*)shaderCode->GetBufferPointer(), shaderCode->GetBufferSize());
				infile.close();
				return shaderCode;
			}

			COM_Ptr<ID3DBlob> CompileShader(std::string& shaderCode, const char* entry, const char* target, const wchar_t* errorOutputFile)
			{
				COM_Ptr<ID3DBlob> shaderBuffer;
				COM_Ptr<ID3DBlob> errorMessage;
				HRESULT hr = D3DCompile(shaderCode.c_str(), shaderCode.length(), nullptr, nullptr, nullptr, entry, target, D3D10_SHADER_ENABLE_STRICTNESS, 0, &shaderBuffer, errorOutputFile ? &errorMessage : nullptr);
				if (FAILED(hr))
				{
					std::ofstream of(errorOutputFile);
					of.write((char*)errorMessage->GetBufferPointer(), errorMessage->GetBufferSize());
					throw std::exception("Failed to compile shader.");
				}
				return shaderBuffer;
			}
		}
	}
}