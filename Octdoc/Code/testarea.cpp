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
	loader.LoadModel(L"Media/cube.omd");
	//loader.CreateSphere(octdoc::mth::float3(), 0.75f, 20, 10, octdoc::gfx::ModelType::PTM);
	//loader.CreateCube(octdoc::mth::float3(-1.0f), octdoc::mth::float3(2.0f), octdoc::gfx::ModelType::PTM);
	//loader.CreateQuad(octdoc::mth::float2(-1.0f), octdoc::mth::float2(2.0f), octdoc::gfx::ModelType::PTM);
	//loader.ChangeModelType(octdoc::gfx::ModelType::PTM);

	//SetTextureMandelbrot(loader.getTexture(0));
	//SetTextureToFile(loader.getTexture(0), L"Media/test.png");
	//SetTextureToFile(loader.getNormalmap(0), L"Media/normalmap.png");
	SetTextureToFile(loader.getTexture(0), L"Media/hinae.gif");

	m_entity = octdoc::gfx::Entity::CreateP(graphics, loader);
	loader.CreateCube(octdoc::mth::float3(-1.0f), octdoc::mth::float3(2.0f), octdoc::gfx::ModelType::PTN);
	m_tv = octdoc::gfx::Entity::CreateP(graphics, loader);
	m_tvScreen = octdoc::gfx::RenderTargetTexture::CreateP(graphics, 512, 512);
	m_tv->getMaterial(0)->setTexture(m_tvScreen);
	m_camera.position.z = -5.0f;
	m_sampler = octdoc::gfx::SamplerState::CreateP(graphics, true, true);
	m_sampler->SetToPixelShader(graphics);
	graphics.EnableAlphaBlending(false);

	/*unsigned uselessWidthCounter = 0;
	auto startTime = std::chrono::steady_clock::now();
	for (int i = 0; i < 1000; i++)
		uselessWidthCounter += octdoc::gfx::Texture::CreateU(graphics, L"F:/Images/5bb16e3e8028ebac00297bc7477c120d.jpg")->getWidth();
	graphics.SetWindowTitle(std::to_wstring(std::chrono::duration<float>(std::chrono::steady_clock::now() - startTime).count()).c_str());
	m_entity->LookUp(float(uselessWidthCounter));*/
	//SDK -> 38.47s for 1000
	//WIC -> 27.32s for 1000
}

void TestArea::OnKeyDown(octdoc::gfx::KeyEvent& e)
{
	if (e.keyCode == octdoc::gfx::Input::KEY_ESCAPE)
		m_gfx->Quit();
	m_cameraController.KeyDown(e.keyCode);
}

void TestArea::OnKeyUp(octdoc::gfx::KeyEvent& e)
{
	m_cameraController.KeyUp(e.keyCode);
}

void TestArea::OnMouseMove(octdoc::gfx::MouseMoveEvent& e)
{
	if (e.lBtnPressed)
		m_cameraController.MouseMove(e.dx, e.dy);
}

void TestArea::OnUpdate(octdoc::gfx::Graphics& graphics, float deltaTime)
{
	octdoc::mth::float4x4 cameraBuffer;
	m_cameraController.Update(deltaTime);
	m_camera.Update();
	cameraBuffer = m_camera.GetCameraMatrix();
	m_cameraBuffer->WriteBuffer(graphics, &cameraBuffer);
	m_cameraBuffer->SetToVertexShader(graphics, 0);

	float lightBuffer[] = {
		1.0f, 1.0f, 1.0f, 1.0f,
		m_camera.position.x, m_camera.position.y, m_camera.position.z,
		0.75f
	};
	m_lightBuffer->WriteBuffer(graphics, lightBuffer);
	m_lightBuffer->SetToPixelShader(graphics, 0);

	graphics.ClearRenderTarget(0.75f, 0.75f, 0.875f);
	m_entity->Render(graphics);
	graphics.Present();
}