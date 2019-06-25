#pragma once

#include "graphics/model.h"
#include "graphics_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			class Model_DX11 :public Model
			{
				SMART_PTR(Model_DX11)
			private:
				COM_Ptr<ID3D11Buffer> m_vertexBuffer;
				COM_Ptr<ID3D11Buffer> m_indexBuffer;

			public:
				Model_DX11(Graphics_DX11& graphics, hlp::ModelLoader& loader);
				static Model_DX11::P CreateP(Graphics_DX11& graphics, hlp::ModelLoader& loader);
				static Model_DX11::U CreateU(Graphics_DX11& graphics, hlp::ModelLoader& loader);

				virtual void SetBuffersToRender(Graphics& graphics) override;
				virtual void RenderGroup(Graphics& graphics, unsigned index) override;
				virtual void RenderAll(Graphics& graphics) override;
			};
		}
	}
}