#pragma once

#include "graphics/entity.h"
#include "graphics/camera.h"
#include "graphics/samplerstate.h"
#include <map>

namespace octdoc
{
	namespace gfx
	{
		class Scene
		{
			SMART_PTR(Scene)
		protected:
			Graphics& m_graphics;
			Camera& m_camera;
			ShaderBuffer::P m_cameraMatrixBuffer;
			ShaderBuffer::P m_worldMatrixBuffer;
			ShaderBuffer::P m_lightBuffer;
			ShaderBuffer::P m_colorBuffer;
			SamplerState::P m_sampler;
			std::vector<Entity::P> m_entities;
			std::map<unsigned, VertexShader::P> m_vertexShaders;
			std::map<unsigned, PixelShader::P> m_pixelShaders;
			mth::float4 m_lightColor;

		public:
			Scene(Graphics& graphics, Camera& camera);
			static Scene::U CreateU(Graphics& graphics, Camera& camera);
			static Scene::P CreateP(Graphics& graphics, Camera& camera);

			void SetAsActiveScene();
			void AddEntity(Entity::P entity);
			void RemoveEntity(Entity::P entity);
			void Clear();

			void Render();

			inline void setLightColor(float red, float green, float blue, float alpha = 1.0f) { m_lightColor = mth::float4(red, green, blue, alpha); }
			inline void setLightColor(mth::float3 color, float alpha = 1.0f) { m_lightColor = mth::float4(color, alpha); }
			inline void setLightColor(mth::float4 color) { m_lightColor = color; }
			inline mth::float4 getLightColor() { return  m_lightColor; }
		};
	}
}