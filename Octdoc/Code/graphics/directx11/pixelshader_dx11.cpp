#include "pixelshader_dx11.h"
#include "graphics/modeltype.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			std::map<std::wstring, PixelShader_DX11::W> PixelShader_DX11::m_loadedShaders;
			void PixelShader_DX11::ClearReleasedShaders()
			{
				for (auto shader : m_loadedShaders)
				{
					if (shader.second.expired())
					{
						m_loadedShaders.erase(shader.first);
						return;
					}
				}
			}
			void PixelShader_DX11::CreatePixelShader(Graphics_DX11& graphics, ID3DBlob* shaderBuffer)
			{
				HRESULT hr = graphics.getDevice()->CreatePixelShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &m_pixelShader);
				if (FAILED(hr))
					throw std::exception("Failed to create pixel shader.");
			}
			PixelShader_DX11::PixelShader_DX11(Graphics_DX11& graphics, const wchar_t* shaderFileName)
			{
				COM_Ptr<ID3DBlob> shaderBuffer = LoadShaderCode(shaderFileName);
				CreatePixelShader(graphics, shaderBuffer);
			}
			PixelShader_DX11::PixelShader_DX11(Graphics_DX11& graphics, unsigned modelType)
			{
				std::string shaderCode;
				shaderCode.reserve(1024);
				shaderCode += "Texture2D tex;Texture2D normalmap;SamplerState ss;\
cbuffer LightBuffer{float4 lightColor;float3 lightPosition;float ambient;};\
cbuffer ColorBuffer{float4 entityColor;};\
struct PixelInputType{";
				if (ModelType::HasPositions(modelType))
					shaderCode += "float4 position:SV_POSITION;";
				if (ModelType::HasNormals(modelType))
					shaderCode += "float3 pos:POSITION;";
				if (ModelType::HasTexcoords(modelType))
					shaderCode += "float2 tex:TEXCOORD;";
				if (ModelType::HasNormals(modelType))
					shaderCode += "float3 normal:NORMAL;";
				if (ModelType::HasTangents(modelType))
					shaderCode += "float3 tangent:TANGENT;";
				shaderCode += "};float4 main(PixelInputType input):SV_TARGET{float4 color=";
				shaderCode += ModelType::HasTexture(modelType) ? "tex.Sample(ss,input.tex);" : "entityColor;";
				if (ModelType::HasNormals(modelType))
				{
					shaderCode += "float3 normal = normalize(input.normal);";
					if (ModelType::HasNormalmap(modelType))
						shaderCode += "float4 bumpMap=normalmap.Sample(ss,input.tex)*2-1;\
input.tangent=normalize(input.tangent-dot(input.tangent,input.normal)*input.normal);\
float3 bitangent=cross(input.tangent,input.normal);\
float3x3 texSpace=float3x3(input.tangent,bitangent,normal);\
normal=normalize(mul(bumpMap,texSpace));";
					shaderCode += "float3 lightDirection=normalize(lightPosition-input.pos);\
float intensity=saturate(dot(normal,lightDirection));\
intensity=ambient+(1-ambient)*intensity;\
color.xyz*=lightColor.xyz*intensity;";
				}
				shaderCode += "return color;}";

				COM_Ptr<ID3DBlob> shaderBuffer = CompileShader(shaderCode, "main", "ps_5_0", L"shadererror.txt");
				CreatePixelShader(graphics, shaderBuffer);
			}
			PixelShader_DX11::~PixelShader_DX11()
			{
				ClearReleasedShaders();
			}
			PixelShader_DX11::P PixelShader_DX11::CreateP(Graphics_DX11& graphics, const wchar_t* shaderFileName)
			{
				return std::make_shared<PixelShader_DX11>(graphics, shaderFileName);
			}
			PixelShader_DX11::U PixelShader_DX11::CreateU(Graphics_DX11& graphics, const wchar_t* shaderFileName)
			{
				return std::make_unique<PixelShader_DX11>(graphics, shaderFileName);
			}
			PixelShader_DX11::P PixelShader_DX11::CreateP(Graphics_DX11& graphics, unsigned modelType)
			{
				return std::make_shared<PixelShader_DX11>(graphics, modelType);
			}
			PixelShader_DX11::U PixelShader_DX11::CreateU(Graphics_DX11& graphics, unsigned modelType)
			{
				return std::make_unique<PixelShader_DX11>(graphics, modelType);
			}
			void PixelShader_DX11::SetShaderToRender(Graphics& graphics)
			{
				ID3D11DeviceContext* context = ((Graphics_DX11&)graphics).getContext();
				context->PSSetShader(m_pixelShader, nullptr, 0);
			}
		}
	}
}