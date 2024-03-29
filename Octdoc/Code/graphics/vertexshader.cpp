#include "directx11/vertexshader_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		VertexShader::~VertexShader() {}
		VertexShader::P VertexShader::CreateP(Graphics& graphics, const wchar_t *shaderFileName, unsigned modelType)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::VertexShader_DX11::CreateP((dx11::Graphics_DX11&)graphics, shaderFileName, modelType);
			}
			return nullptr;
		}
		VertexShader::U VertexShader::CreateU(Graphics & graphics, const wchar_t * shaderFileName, unsigned modelType)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::VertexShader_DX11::CreateU((dx11::Graphics_DX11&)graphics, shaderFileName, modelType);
			}
			return nullptr;
		}
		VertexShader::P VertexShader::CreateP(Graphics& graphics, unsigned modelType)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::VertexShader_DX11::CreateP((dx11::Graphics_DX11&)graphics, modelType);
			}
			return nullptr;
		}
		VertexShader::U VertexShader::CreateU(Graphics& graphics, unsigned modelType)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::VertexShader_DX11::CreateU((dx11::Graphics_DX11&)graphics, modelType);
			}
			return nullptr;
		}
		VertexShader::P VertexShader::CreatePFromString(Graphics& graphics, const char* shaderCode, unsigned modelType)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::VertexShader_DX11::CreatePFromString((dx11::Graphics_DX11&)graphics, shaderCode, modelType);
			}
			return nullptr;
		}
		VertexShader::U VertexShader::CreateUFromString(Graphics& graphics, const char* shaderCode, unsigned modelType)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::VertexShader_DX11::CreateUFromString((dx11::Graphics_DX11&)graphics, shaderCode, modelType);
			}
			return nullptr;
		}
	}
}