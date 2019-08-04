#include "scene.h"

namespace octdoc
{
	namespace gfx
	{
		Scene::Scene(Graphics& graphics, Camera& camera) :
			m_graphics(graphics), m_camera(camera), m_lightColor(1.0f, 1.0f, 1.0f, 1.0f)
		{
			m_cameraMatrixBuffer = ShaderBuffer::CreateP(graphics, sizeof(mth::float4x4));
			m_worldMatrixBuffer = ShaderBuffer::CreateP(graphics, sizeof(mth::float4x4));
			m_lightBuffer = ShaderBuffer::CreateP(graphics, sizeof(float) * 8);
			m_colorBuffer = ShaderBuffer::CreateP(graphics, sizeof(mth::float4));
			m_sampler = SamplerState::CreateP(graphics, true, true);
			SetAsActiveScene();
		}
		Scene::U Scene::CreateU(Graphics& graphics, Camera& camera)
		{
			return std::make_unique<Scene>(graphics, camera);
		}
		Scene::P Scene::CreateP(Graphics& graphics, Camera& camera)
		{
			return std::make_shared<Scene>(graphics, camera);
		}
		void Scene::SetAsActiveScene()
		{
			m_cameraMatrixBuffer->SetToVertexShader(m_graphics, 0);
			m_worldMatrixBuffer->SetToVertexShader(m_graphics, 1);
			m_lightBuffer->SetToPixelShader(m_graphics, 0);
			m_colorBuffer->SetToPixelShader(m_graphics, 1);
			m_sampler->SetToPixelShader(m_graphics);
		}
		void Scene::AddEntity(Entity::P entity)
		{
			m_entities.push_back(entity);
			unsigned modelType = entity->getModel()->getModelType();
			if (m_vertexShaders.end() == m_vertexShaders.find(modelType))
				m_vertexShaders[modelType] = VertexShader::CreateP(m_graphics, modelType);
			if (m_pixelShaders.end() == m_pixelShaders.find(modelType))
				m_pixelShaders[modelType] = PixelShader::CreateP(m_graphics, modelType);
		}
		void Scene::RemoveEntity(Entity::P entity)
		{
			for (auto e = m_entities.begin(); e != m_entities.end(); e++)
			{
				if (*e == entity)
				{
					m_entities.erase(e);
					return;
				}
			}
		}
		void Scene::Clear()
		{
			m_entities.clear();
			m_vertexShaders.clear();
			m_pixelShaders.clear();
		}
		void Scene::Render()
		{
			mth::float4x4 cameraBuffer = m_camera.GetCameraMatrix();
			m_cameraMatrixBuffer->WriteBuffer(m_graphics, &cameraBuffer);
			float lightBuffer[] = {
				m_lightColor.x, m_lightColor.y, m_lightColor.z, m_lightColor.w, 
				m_camera.position.x, m_camera.position.y, m_camera.position.z,
				0.7f
			};
			m_lightBuffer->WriteBuffer(m_graphics, lightBuffer);

			for (auto& e : m_entities)
			{
				unsigned modelType = e->getModel()->getModelType();
				m_vertexShaders[modelType]->SetShaderToRender(m_graphics);
				m_pixelShaders[modelType]->SetShaderToRender(m_graphics);
				mth::float4x4 worldMatrix = e->GetWorldMatrix();
				m_worldMatrixBuffer->WriteBuffer(m_graphics, &worldMatrix);
				mth::float4 color = e->getColor();
				m_colorBuffer->WriteBuffer(m_graphics, &color);
				e->Render(m_graphics);
			}
		}
	}
}