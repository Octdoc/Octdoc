#pragma once

#include "graphics/vertexshader.h"
#include "shaderbase_dx11.h"
#include "shaderbuffer_dx11.h"
#include <map>
#include <string>

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			class VertexShader_DX11 :public VertexShader
			{
				SMART_PTR(VertexShader_DX11)
			private:
				static std::map<std::wstring, VertexShader_DX11::W> m_loadedShaders;
				static void ClearReleasedShaders();

			private:
				COM_Ptr<ID3D11VertexShader> m_vertexShader;
				COM_Ptr<ID3D11InputLayout> m_inputLayout;

			private:
				void CreateVertexShader(Graphics_DX11& graphics, ID3DBlob* shaderBuffer);
				void CreateInputLayout(Graphics_DX11& graphics, ID3DBlob* shaderBuffer);

			public:
				VertexShader_DX11(Graphics_DX11& graphics, LPCWSTR shaderFileName, unsigned modelType);
				VertexShader_DX11(Graphics_DX11& graphics, unsigned modelType);
				~VertexShader_DX11();
				static VertexShader_DX11::P CreateP(Graphics_DX11& graphics, LPCWSTR shaderFileName, unsigned modelType);
				static VertexShader_DX11::U CreateU(Graphics_DX11& graphics, LPCWSTR shaderFileName, unsigned modelType);
				static VertexShader_DX11::P CreateP(Graphics_DX11& graphics, unsigned modelType);
				static VertexShader_DX11::U CreateU(Graphics_DX11& graphics, unsigned modelType);

				void SetShaderToRender(Graphics& graphics);
			};
		}
	}
}