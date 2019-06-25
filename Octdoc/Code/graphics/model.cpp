#include "directx11/model_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		Model::P Model::CreateP(Graphics& graphics, hlp::ModelLoader& loader)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Model_DX11::CreateP((dx11::Graphics_DX11&)graphics, loader);
			}
			return nullptr;
		}
		Model::U Model::CreateU(Graphics& graphics, hlp::ModelLoader& loader)
		{
			switch (graphics.getGfxEngine())
			{
			case GraphicsEngine::DirectX11:
				return dx11::Model_DX11::CreateU((dx11::Graphics_DX11&)graphics, loader);
			}
			return nullptr;
		}
	}
}
