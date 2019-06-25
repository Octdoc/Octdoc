#pragma once

#include "material.h"
#include "model.h"

namespace octdoc
{
	namespace gfx
	{
		class Entity :public mth::Position
		{
			SMART_PTR(Entity)
		private:
			Model::P m_model;
			std::vector<Material::P> m_materials;
			ShaderBuffer::P m_worldMatrixBuffer;
			ShaderBuffer::P m_colorBuffer;
			mth::float4 m_color;

		public:
			Entity(Graphics& graphics, hlp::ModelLoader& loader);
			static Entity::P CreateP(Graphics& graphics, hlp::ModelLoader& loader);
			static Entity::U CreateU(Graphics& graphics, hlp::ModelLoader& loader);

			void Render(Graphics& graphics);
			Model::P getModel() { return m_model; }
			unsigned getMaterialCount() { return (unsigned)m_materials.size(); }
			Material::P getMaterial(unsigned index) { return m_materials[index]; }
		};
	}
}