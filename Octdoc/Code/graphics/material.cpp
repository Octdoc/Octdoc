#include "material.h"
#include "materialtype.h"

namespace octdoc
{
	namespace gfx
	{
		Material::Material(Graphics& graphics, hlp::ModelLoader& loader, unsigned materialIndex)
		{
			try
			{
				if (ModelType::HasTexture(loader.getModelType()))
				{
					hlp::ModelData::MaterialData::Texture& texture = loader.getTexture(materialIndex);
					if (texture.loaded)
						m_texture = Texture::CreateP(graphics, texture.data.data(), texture.width, texture.height);
					else if (!texture.filename.empty())
						m_texture = Texture::CreateP(graphics, texture.filename.c_str());
				}

				if (ModelType::HasNormalmap(loader.getModelType()))
				{
					hlp::ModelData::MaterialData::Texture& normalmap = loader.getNormalmap(materialIndex);
					if (normalmap.loaded)
						m_normalmap = Texture::CreateP(graphics, normalmap.data.data(), normalmap.width, normalmap.height);
					else if (!normalmap.filename.empty())
						m_normalmap = Texture::CreateP(graphics, normalmap.filename.c_str());
				}
			}
			catch (std::exception e)
			{
				
			}
		}
		Material::P Material::CreateP(Graphics& graphics, hlp::ModelLoader& loader, unsigned materialIndex)
		{
			return std::make_shared<Material>(graphics, loader, materialIndex);
		}
		Material::U Material::CreateU(Graphics& graphics, hlp::ModelLoader& loader, unsigned materialIndex)
		{
			return std::make_unique<Material>(graphics, loader, materialIndex);
		}
		void Material::SetToRender(Graphics& graphics)
		{
			if (m_texture)
				m_texture->SetToRender(graphics, 0);
			if (m_normalmap)
				m_normalmap->SetToRender(graphics, 1);
		}
	}
}