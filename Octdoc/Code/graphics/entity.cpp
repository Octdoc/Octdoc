#include "entity.h"

namespace octdoc
{
	namespace gfx
	{
		Entity::Entity(Graphics& graphics, hlp::ModelLoader& loader) :
			m_color(1.0f)
		{
			m_model = Model::CreateP(graphics, loader);
			m_materials.resize(loader.getVertexGroupCount());
			for (unsigned i = 0; i < loader.getVertexGroupCount(); i++)
				m_materials[i] = Material::CreateP(graphics, loader, loader.getVertexGroup(i).materialIndex);
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
			for (unsigned i = 0; i < m_materials.size(); i++)
			{
				m_materials[i]->SetToRender(graphics);
				m_model->RenderGroup(graphics, i);
			}
		}
	}
}