#include "directx11/pixelshader_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		PixelShader::P PixelShader::CreateP(Graphics& graphics, const wchar_t *shaderFileName)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::PixelShader_DX11::CreateP((dx11::Graphics_DX11&)graphics, shaderFileName);
			}
			return nullptr;
		}
		PixelShader::U PixelShader::CreateU(Graphics& graphics, const wchar_t *shaderFileName)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::PixelShader_DX11::CreateU((dx11::Graphics_DX11&)graphics, shaderFileName);
			}
			return nullptr;
		}
		PixelShader::P PixelShader::CreateP(Graphics& graphics, unsigned modelType)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::PixelShader_DX11::CreateP((dx11::Graphics_DX11&)graphics, modelType);
			}
			return nullptr;
		}
		PixelShader::U PixelShader::CreateU(Graphics& graphics, unsigned modelType)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::PixelShader_DX11::CreateU((dx11::Graphics_DX11&)graphics, modelType);
			}
			return nullptr;
		}
	}
}