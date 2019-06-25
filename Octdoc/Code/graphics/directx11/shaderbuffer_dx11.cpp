#include "shaderbuffer_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			ShaderBuffer_DX11::ShaderBuffer_DX11(Graphics_DX11& graphics, unsigned size)
			{
				D3D11_BUFFER_DESC bufferDesc;
				HRESULT hr;
				ID3D11Device *device = graphics.getDevice();
				m_bufferSize = size;

				bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
				bufferDesc.ByteWidth = m_bufferSize;
				bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				bufferDesc.MiscFlags = 0;
				bufferDesc.StructureByteStride = 0;
				bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

				hr = device->CreateBuffer(&bufferDesc, nullptr, &m_buffer);
				if (FAILED(hr))
					throw std::exception("Failed to create constant buffer");
			}
			ShaderBuffer_DX11::P ShaderBuffer_DX11::CreateP(Graphics_DX11& graphics, unsigned size)
			{
				return std::make_shared<ShaderBuffer_DX11>(graphics, size);
			}
			ShaderBuffer_DX11::U ShaderBuffer_DX11::CreateU(Graphics_DX11& graphics, unsigned size)
			{
				return std::make_unique<ShaderBuffer_DX11>(graphics, size);
			}
			bool ShaderBuffer_DX11::WriteBuffer(Graphics& graphics, void *data)
			{
				ID3D11DeviceContext *context = ((Graphics_DX11&)graphics).getContext();
				D3D11_MAPPED_SUBRESOURCE resource;
				if (FAILED(context->Map(m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource)))
					return false;
				memcpy(resource.pData, data, m_bufferSize);
				context->Unmap(m_buffer, 0);
				return true;
			}
			void ShaderBuffer_DX11::SetToVertexShader(Graphics& graphics, unsigned index)
			{
				ID3D11DeviceContext *context = ((Graphics_DX11&)graphics).getContext();
				context->VSSetConstantBuffers(index, 1, &m_buffer);
			}
			void ShaderBuffer_DX11::SetToPixelShader(Graphics& graphics, unsigned index)
			{
				ID3D11DeviceContext *context = ((Graphics_DX11&)graphics).getContext();
				context->PSSetConstantBuffers(index, 1, &m_buffer);
			}
		}
	}
}
