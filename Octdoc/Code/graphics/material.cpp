#include "material.h"
#include "materialtype.h"

namespace octdoc
{
	namespace gfx
	{
		Material::Material(Graphics& graphics, hlp::ModelLoader& loader, unsigned materialIndex)
		{
			//m_vertexShader = VertexShader::CreateP(graphics, MaterialType::VertexShaderName(loader.getMaterialType(materialIndex)), loader.getModelType());
			//m_pixelShader = PixelShader::CreateP(graphics, MaterialType::PixelShaderName(loader.getMaterialType(materialIndex)));
			m_vertexShader = VertexShader::CreateP(graphics, loader.getModelType());
			m_pixelShader = PixelShader::CreateP(graphics, loader.getModelType());

			hlp::ModelData::MaterialData::Texture& texture = loader.getTexture(materialIndex);
			if (texture.loaded)
				m_texture = Texture::CreateP(graphics, texture.data.data(), texture.width, texture.height);
			else if (!texture.filename.empty())
				m_texture = Texture::CreateP(graphics, texture.filename.c_str());

			hlp::ModelData::MaterialData::Texture& normalmap = loader.getNormalmap(materialIndex);
			if (normalmap.loaded)
				m_normalmap = Texture::CreateP(graphics, normalmap.data.data(), normalmap.width, normalmap.height);
			else if (!normalmap.filename.empty())
				m_normalmap = Texture::CreateP(graphics, normalmap.filename.c_str());
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
			if (m_vertexShader)
				m_vertexShader->SetShaderToRender(graphics);
			if (m_pixelShader)
				m_pixelShader->SetShaderToRender(graphics);
			if (m_texture)
				m_texture->SetToRender(graphics, 0);
			if (m_normalmap)
				m_normalmap->SetToRender(graphics, 1);
		}
	}
}