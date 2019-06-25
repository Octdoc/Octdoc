#include "model_dx11.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			Model_DX11::Model_DX11(Graphics_DX11& graphics, hlp::ModelLoader& loader)
			{
				HRESULT hr;
				D3D11_BUFFER_DESC bufferDesc{};
				D3D11_SUBRESOURCE_DATA subResourceData{};
				ID3D11Device *device = ((Graphics_DX11&)graphics).getDevice();

				m_modelType = loader.getModelType();
				m_vertexSizeInBytes = loader.getVertexSizeInBytes();
				m_vertexCount = loader.getVertexCount();
				m_indexCount = loader.getIndexCount();

				bufferDesc.Usage = D3D11_USAGE_DEFAULT;
				bufferDesc.ByteWidth = m_vertexCount * m_vertexSizeInBytes;
				bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				subResourceData.pSysMem = loader.getVertices();

				hr = device->CreateBuffer(&bufferDesc, &subResourceData, &m_vertexBuffer);
				if (FAILED(hr))
					throw std::exception("Failed to create vertex buffer");

				bufferDesc.ByteWidth = m_indexCount * sizeof(UINT);
				bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				subResourceData.pSysMem = loader.getIndices();

				hr = device->CreateBuffer(&bufferDesc, &subResourceData, &m_indexBuffer);
				if (FAILED(hr))
					throw std::exception("Failed to create index buffer");

				for (UINT i = 0; i < loader.getVertexGroupCount(); i++)
				{
					VertexGroup vg = loader.getVertexGroup(i);
					m_groups.push_back({ vg.startIndex, vg.indexCount });
				}
			}

			Model_DX11::P Model_DX11::CreateP(Graphics_DX11& graphics, hlp::ModelLoader& loader)
			{
				return std::make_shared<Model_DX11>(graphics, loader);
			}
			Model_DX11::U Model_DX11::CreateU(Graphics_DX11& graphics, hlp::ModelLoader& loader)
			{
				return std::make_unique<Model_DX11>(graphics, loader);
			}

			void Model_DX11::SetBuffersToRender(Graphics& graphics)
			{
				UINT stride = m_vertexSizeInBytes;
				UINT offset = 0;
				ID3D11DeviceContext *context = ((Graphics_DX11&)graphics).getContext();

				context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
				context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
			}
			void Model_DX11::RenderGroup(Graphics& graphics, unsigned index)
			{
				((Graphics_DX11&)graphics).getContext()->DrawIndexed(m_groups[index].indexCount, m_groups[index].startIndex, 0);
			}
			void Model_DX11::RenderAll(Graphics& graphics)
			{
				SetBuffersToRender(graphics);
				((Graphics_DX11&)graphics).getContext()->DrawIndexed(m_indexCount, 0, 0);
			}
		}
	}
}