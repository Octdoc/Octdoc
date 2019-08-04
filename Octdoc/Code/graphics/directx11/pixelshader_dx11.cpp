#include "pixelshader_dx11.h"
#include "graphics/modeltype.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			hlp::LoadOnceContainer<PixelShader_DX11> PixelShader_DX11::m_loadedShaders;
			void PixelShader_DX11::ClearReleasedShaders()
			{
				m_loadedShaders.Cleanup();
			}
			COM_Ptr<ID3DBlob> PixelShader_DX11::CompilePixelShader(Graphics_DX11& graphics, std::string& shaderCode)
			{
				const char* target = nullptr;
				switch (graphics.getFeatureLevel())
				{
				case D3D_FEATURE_LEVEL_10_0:
					target = "ps_4_0";
					break;
				case D3D_FEATURE_LEVEL_10_1:
					target = "ps_4_1";
					break;
				case D3D_FEATURE_LEVEL_11_0:
				case D3D_FEATURE_LEVEL_11_1:
					target = "ps_5_0";
					break;
				default:
					target = "ps_4_0";
				}
				return CompileShader(shaderCode, "main", target, L"shadererror.txt");
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
color.xyz*=intensity;";
				}
				shaderCode += "return color*lightColor;}";

				COM_Ptr<ID3DBlob> shaderBuffer = CompilePixelShader(graphics, shaderCode);
				CreatePixelShader(graphics, shaderBuffer);
			}
			PixelShader_DX11::PixelShader_DX11(Graphics_DX11& graphics, const char* shaderCode)
			{
				std::string code(shaderCode);
				COM_Ptr<ID3DBlob> shaderBuffer = CompilePixelShader(graphics, code);
				CreatePixelShader(graphics, shaderBuffer);
			}
			PixelShader_DX11::~PixelShader_DX11()
			{
				ClearReleasedShaders();
			}
			PixelShader_DX11::P PixelShader_DX11::CreateP(Graphics_DX11& graphics, const wchar_t* shaderFileName)
			{
				PixelShader_DX11::P ps = m_loadedShaders.Find(shaderFileName);
				if (ps == nullptr)
				{
					ps = std::make_shared<PixelShader_DX11>(graphics, shaderFileName);
					m_loadedShaders.Add(shaderFileName, ps);
				}
				return ps;
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
			PixelShader_DX11::P PixelShader_DX11::CreatePFromString(Graphics_DX11& graphics, const char* shaderCode)
			{
				return std::make_shared<PixelShader_DX11>(graphics, shaderCode);
			}
			PixelShader_DX11::U PixelShader_DX11::CreateUFromString(Graphics_DX11& graphics, const char* shaderCode)
			{
				return std::make_unique<PixelShader_DX11>(graphics, shaderCode);
			}
			void PixelShader_DX11::SetShaderToRender(Graphics& graphics)
			{
				ID3D11DeviceContext* context = ((Graphics_DX11&)graphics).getContext();
				context->PSSetShader(m_pixelShader, nullptr, 0);
			}
		}
	}
}