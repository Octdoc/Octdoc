#include "entity.h"

namespace octdoc
{
	namespace gfx
	{
		Entity::Entity(Graphics& graphics, hlp::ModelLoader& loader) :
			m_color(1.0f)
		{
			m_model = Model::CreateP(graphics, loader);
			m_materials.resize(loader.getMaterialCount());
			for (unsigned i = 0; i < loader.getMaterialCount(); i++)
				m_materials[i] = Material::CreateP(graphics, loader, i);
			m_worldMatrixBuffer = ShaderBuffer::CreateP(graphics, sizeof(mth::float4x4));
			m_colorBuffer = ShaderBuffer::CreateP(graphics, sizeof(mth::float4));
		}
		Entity::P Entity::CreateP(Graphics& graphics, hlp::ModelLoader& loader)
		{
			return std::make_shared<Entity>(graphics, loader);
		}
		Entity::U Entity::CreateU(Graphics& graphics, hlp::ModelLoader& loader)
		{
			return std::make_unique<Entity>(graphics, loader);
		}
		void Entity::Render(Graphics& graphics)
		{
			m_model->SetBuffersToRender(graphics);
			mth::float4x4 worldMatrix = GetWorldMatrix();
			m_worldMatrixBuffer->WriteBuffer(graphics, &worldMatrix);
			m_worldMatrixBuffer->SetToVertexShader(graphics, 1);
			m_colorBuffer->WriteBuffer(graphics, &m_color);
			m_colorBuffer->SetToPixelShader(graphics, 1);

			for (unsigned i = 0; i < m_materials.size(); i++)
			{
				m_materials[i]->SetToRender(graphics);
				m_model->RenderGroup(graphics, i);
			}
		}
	}
}