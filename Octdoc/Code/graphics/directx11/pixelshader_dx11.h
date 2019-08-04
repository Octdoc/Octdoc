#pragma once

#include "graphics/pixelshader.h"
#include "shaderbase_dx11.h"
#include "shaderbuffer_dx11.h"
#include "helpers/loadoncecontainer.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			class PixelShader_DX11 :public PixelShader
			{
				SMART_PTR(PixelShader_DX11)
			private:
				static hlp::LoadOnceContainer<PixelShader_DX11> m_loadedShaders;
				static void ClearReleasedShaders();

			private:
				COM_Ptr<ID3D11PixelShader> m_pixelShader;

			private:
				COM_Ptr<ID3DBlob> CompilePixelShader(Graphics_DX11& graphics, std::string& shaderCode);
				void CreatePixelShader(Graphics_DX11& graphics, ID3DBlob* shaderBuffer);

			public:
				PixelShader_DX11(Graphics_DX11& graphics, const wchar_t* shaderFileName);
				PixelShader_DX11(Graphics_DX11& graphics, unsigned modelType);
				PixelShader_DX11(Graphics_DX11& graphics, const char* shaderCode);
				virtual ~PixelShader_DX11() override;
				static PixelShader_DX11::P CreateP(Graphics_DX11& graphics, const wchar_t* shaderFileName);
				static PixelShader_DX11::U CreateU(Graphics_DX11& graphics, const wchar_t* shaderFileName);
				static PixelShader_DX11::P CreateP(Graphics_DX11& graphics, unsigned modelType);
				static PixelShader_DX11::U CreateU(Graphics_DX11& graphics, unsigned modelType);
				static PixelShader_DX11::P CreatePFromString(Graphics_DX11& graphics, const char* shaderCode);
				static PixelShader_DX11::U CreateUFromString(Graphics_DX11& graphics, const char* shaderCode);

				virtual void SetShaderToRender(Graphics& graphics) override;
			};
		}
	}
}