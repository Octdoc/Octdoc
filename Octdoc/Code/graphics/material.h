#pragma once

#include "texture.h"
#include "rendertargettexture.h"
#include "vertexshader.h"
#include "pixelshader.h"
#include "shaderbuffer.h"
#include "helpers/modelloader.h"

namespace octdoc
{
	namespace gfx
	{
		class Material
		{
			SMART_PTR(Material)
		private:
			Texture::P m_texture;
			Texture::P m_normalmap;

		public:
			Material(Graphics& graphics, hlp::ModelLoader& loader, unsigned materialIndex);
			static Material::P CreateP(Graphics& graphics, hlp::ModelLoader& loader, unsigned materialIndex);
			static Material::U CreateU(Graphics& graphics, hlp::ModelLoader& loader, unsigned materialIndex);

			inline void setTexture(Texture::P texture) { m_texture = texture; }
			inline void setNormalmap(Texture::P normalmap) { m_normalmap = normalmap; }
			inline Texture::P getTexture() { return m_texture; }
			inline Texture::P getNormalmap() { return m_normalmap; }
			virtual void SetToRender(Graphics& graphics);
		};
	}
}