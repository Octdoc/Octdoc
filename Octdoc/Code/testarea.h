#pragma once

#include "graphics/application.h"
#include "graphics/entity.h"
#include "graphics/camera.h"
#include "graphics/samplerstate.h"
#include "helpers/firstpersoncontroller.h"

class TestArea : public octdoc::gfx::Application
{
	octdoc::gfx::Graphics *m_gfx;
	octdoc::gfx::Camera m_camera;
	octdoc::hlp::FirstPersonController m_cameraController;
	octdoc::gfx::Entity::P m_entity;
	octdoc::gfx::RenderTargetTexture::P m_tvScreen;
	octdoc::gfx::Entity::P m_tv;
	octdoc::gfx::ShaderBuffer::P m_cameraBuffer;
	octdoc::gfx::ShaderBuffer::P m_lightBuffer;
	octdoc::gfx::SamplerState::P m_sampler;

private:
	void SetTextureMandelbrot(octdoc::hlp::ModelData::MaterialData::Texture& texture);
	void SetTextureToFile(octdoc::hlp::ModelData::MaterialData::Texture& texture, const wchar_t* filename);

public:
	TestArea();
	virtual void OnStart(octdoc::gfx::Graphics& graphics) override;
	virtual void OnKeyDown(octdoc::gfx::KeyEvent& e) override;
	virtual void OnKeyUp(octdoc::gfx::KeyEvent& e) override;
	virtual void OnMouseMove(octdoc::gfx::MouseMoveEvent& e) override;
	virtual void OnUpdate(octdoc::gfx::Graphics& graphics, float deltaTime) override;
};