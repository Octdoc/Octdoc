#include "testarea.h"
#include <chrono>

void TestArea::SetTextureMandelbrot(octdoc::hlp::ModelData::MaterialData::Texture& texture)
{
	int textureWidth = 512;
	int textureHeight = 512;
	std::vector<unsigned char> img(textureWidth * textureHeight * 4);
	for (int x = 0; x < textureWidth; x++)
		for (int y = 0; y < textureHeight; y++)
		{
			float i;
			float cx = ((float)x - float(textureWidth) * 0.7f) / float(textureWidth) * 2.5f;
			float cy = ((float)y - float(textureHeight) * 0.5f) / float(textureHeight) * 2.5f;
			float tx, ty, zx = 0.0f, zy = 0.0f;

			for (i = 0.0f; i < 256.0f; i++)
			{
				tx = zx * zx - zy * zy;
				ty = 2.0f * zx * zy;
				zx = tx + cx;
				zy = ty + cy;
				if (zx * zx + zy * zy > 4.0f)
					break;
			}

			i /= 256.0f;
			float r = fabsf(6.0f * i - 3.0f) - 1.0f;
			float g = 2.0f - fabsf(6.0f * i - 2.0f);
			float b = 2.0f - fabsf(6.0f * i - 4.0f);
			r *= (1.0f - r * 0.49f);
			g *= (1.0f - r * 0.49f);
			b *= (1.0f - r * 0.49f);
			if (r < 0.0f)r = 0.0f;
			else if (r > 1.0f)r = 1.0f;
			if (g < 0.0f)g = 0.0f;
			else if (g > 1.0f)g = 1.0f;
			if (b < 0.0f)b = 0.0f;
			else if (b > 1.0f)b = 1.0f;

			img[(x + y * textureWidth) * 4 + 0] = 255 - (char)(r * 255.0f);
			img[(x + y * textureWidth) * 4 + 1] = 255 - (char)(g * 255.0f);
			img[(x + y * textureWidth) * 4 + 2] = 255 - (char)(b * 255.0f);
			img[(x + y * textureWidth) * 4 + 3] = 255;
		}
	texture.filename.clear();
	texture.data.swap(img);
	texture.width = textureWidth;
	texture.height = textureHeight;
	texture.loaded = true;
}

void TestArea::SetTextureToFile(octdoc::hlp::ModelData::MaterialData::Texture& texture, const wchar_t* filename)
{
	texture.filename = filename;
	texture.data.clear();
	texture.width = 0;
	texture.height = 0;
	texture.loaded = false;
}

TestArea::TestArea() :m_cameraController(m_camera) {}

void TestArea::OnStart(octdoc::gfx::Graphics& graphics)
{
	m_gfx = &graphics;
	m_cameraBuffer = octdoc::gfx::ShaderBuffer::CreateP(graphics, sizeof(octdoc::mth::float4x4));
	m_lightBuffer = octdoc::gfx::ShaderBuffer::CreateP(graphics, sizeof(float) * 8);
	octdoc::hlp::ModelLoader loader;

	loader.CreateSphere(octdoc::mth::float3(), 1.0f, 200, 100, octdoc::gfx::ModelType::PN);
	//loader.Transform(octdoc::mth::float4x4::Scaling(0.75f, 1.5f, 1.0f));
	m_entity = octdoc::gfx::Entity::CreateP(graphics, loader);
	m_entity->scale = octdoc::mth::float3(0.75f, 1.5f, 1.0f);
	m_entity->MoveUp(5.0f);

	loader.CreateQuad(octdoc::mth::float2(-5.0f, -5.0f), octdoc::mth::float2(4.6f, 4.6f), octdoc::gfx::ModelType::PN);
	//loader.CreateCube(octdoc::mth::float3(-1.0f, -1.0f, -1.0f), octdoc::mth::float3(2.0f, 2.0f, 2.0f), octdoc::gfx::ModelType::PN);
	//loader.CreateSphere(octdoc::mth::float3(-1.0f, -1.0f, -1.0f), 1.0f, 20, 10, octdoc::gfx::ModelType::PN);
	//loader.Transform(octdoc::mth::float4x4::Rotation(1.0f, 2.0f, 3.0f));
	m_floor = octdoc::gfx::Entity::CreateP(graphics, loader);
	m_floor->setColor(octdoc::mth::float4(0.3f, 0.9f, 0.4f, 1.0f));
	loader.MakeHitboxFromVertices();
	m_square = octdoc::physx::Collider::CreateU(loader);

	m_camera.position.z = -5.0f;
	m_sampler = octdoc::gfx::SamplerState::CreateP(graphics, true, true);
	m_sampler->SetToPixelShader(graphics);

	m_keyFlags = 0;
}

void TestArea::OnKeyDown(octdoc::gfx::KeyEvent& e)
{
	if (e.keyCode == octdoc::gfx::Input::KEY_ESCAPE)
		m_gfx->Quit();
	m_cameraController.KeyDown(e.keyCode);

	switch (e.keyCode)
	{
	case octdoc::gfx::Input::KEY_UP:
		m_keyFlags |= 1;
		break;
	case octdoc::gfx::Input::KEY_LEFT:
		m_keyFlags |= 2;
		break;
	case octdoc::gfx::Input::KEY_DOWN:
		m_keyFlags |= 4;
		break;
	case octdoc::gfx::Input::KEY_RIGHT:
		m_keyFlags |= 8;
		break;
	case octdoc::gfx::Input::KEY_PRIOR:
		m_keyFlags |= 16;
		break;
	case octdoc::gfx::Input::KEY_NEXT:
		m_keyFlags |= 32;
		break;
	}
}

void TestArea::OnKeyUp(octdoc::gfx::KeyEvent& e)
{
	m_cameraController.KeyUp(e.keyCode);

	switch (e.keyCode)
	{
	case octdoc::gfx::Input::KEY_UP:
		m_keyFlags &= ~1;
		break;
	case octdoc::gfx::Input::KEY_LEFT:
		m_keyFlags &= ~2;
		break;
	case octdoc::gfx::Input::KEY_DOWN:
		m_keyFlags &= ~4;
		break;
	case octdoc::gfx::Input::KEY_RIGHT:
		m_keyFlags &= ~8;
		break;
	case octdoc::gfx::Input::KEY_PRIOR:
		m_keyFlags &= ~16;
		break;
	case octdoc::gfx::Input::KEY_NEXT:
		m_keyFlags &= ~32;
		break;
	}
}

void TestArea::OnMouseMove(octdoc::gfx::MouseMoveEvent& e)
{
	if (e.lBtnPressed)
		m_cameraController.MouseMove(e.dx, e.dy);
}

void TestArea::OnUpdate(octdoc::gfx::Graphics& graphics, float deltaTime)
{
	if (deltaTime > 0.02f) deltaTime = 0.02f;
	octdoc::mth::float4x4 cameraBuffer;
	m_cameraController.Update(deltaTime);
	m_camera.Update();
	cameraBuffer = m_camera.GetCameraMatrix();
	m_cameraBuffer->WriteBuffer(graphics, &cameraBuffer);
	m_cameraBuffer->SetToVertexShader(graphics, 0);

	float lightBuffer[] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		m_camera.position.x, m_camera.position.y, m_camera.position.z,
		0.7f
	};
	m_lightBuffer->WriteBuffer(graphics, lightBuffer);
	m_lightBuffer->SetToPixelShader(graphics, 0);

	float speed = 4.0f * deltaTime;
	octdoc::mth::float3 movement;
	if (m_keyFlags & 1)
		movement.z += speed;
	if (m_keyFlags & 2)
		movement.x -= speed;
	if (m_keyFlags & 4)
		movement.z -= speed;
	if (m_keyFlags & 8)
		movement.x += speed;
	if (m_keyFlags & 16)
		movement.y += speed;
	if (m_keyFlags & 32)
		movement.y -= speed;
	octdoc::physx::CollisionData collData;
	movement = octdoc::mth::float3x3::RotationY(m_camera.rotation.y) * movement;
	m_square->CollidesWithEllipsoid((octdoc::mth::Position<double>)((octdoc::mth::Position<float>) * m_entity), (octdoc::mth::double3)movement, collData);
	m_entity->Move(movement * (collData.time - 1e-3f));

	graphics.ClearRenderTarget(0.75f, 0.75f, 0.875f);
	m_floor->Render(graphics);
	m_entity->Render(graphics);
	graphics.Present();
}