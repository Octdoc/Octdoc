#pragma once

#include "material.h"
#include "model.h"

namespace octdoc
{
	namespace gfx
	{
		class Entity :public mth::Position<float>
		{
			SMART_PTR(Entity)
		private:
			Model::P m_model;
			std::vector<Material::P> m_materials;
			mth::float4 m_color;

		public:
			Entity(Graphics& graphics, hlp::ModelLoader& loader);
			static Entity::P CreateP(Graphics& graphics, hlp::ModelLoader& loader);
			static Entity::U CreateU(Graphics& graphics, hlp::ModelLoader& loader);

			void Render(Graphics& graphics);
			inline Model::P getModel() { return m_model; }
			inline unsigned getMaterialCount() { return (unsigned)m_materials.size(); }
			inline Material::P getMaterial(unsigned index) { return m_materials[index]; }
			inline mth::float4 getColor() { return m_color; }
			inline void setColor(mth::float4 color) { m_color = color; }
		};
	}
}