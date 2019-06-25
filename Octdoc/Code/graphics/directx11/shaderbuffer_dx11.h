#pragma once

#include "graphics/shaderbuffer.h"
#include "graphics_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			class ShaderBuffer_DX11 :public ShaderBuffer
			{
				SMART_PTR(ShaderBuffer_DX11)
			private:
				COM_Ptr<ID3D11Buffer> m_buffer;

			public:
				ShaderBuffer_DX11(Graphics_DX11& graphics, unsigned size);
				static ShaderBuffer_DX11::P CreateP(Graphics_DX11& graphics, unsigned size);
				static ShaderBuffer_DX11::U CreateU(Graphics_DX11& graphics, unsigned size);

				virtual bool WriteBuffer(Graphics& graphics, void *data) override;
				virtual void SetToVertexShader(Graphics& graphics, unsigned index = 0) override;
				virtual void SetToPixelShader(Graphics& graphics, unsigned index = 0) override;
			};
		}
	}
}