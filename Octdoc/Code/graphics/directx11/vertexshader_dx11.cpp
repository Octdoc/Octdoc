#include "vertexshader_dx11.h"
#include "shaderbase_dx11.h"
#include "graphics/modeltype.h"

namespace octdoc
{
	namespace gfx
	{
		namespace dx11
		{
			hlp::LoadOnceContainer<VertexShader_DX11> VertexShader_DX11::m_loadedShaders;
			void VertexShader_DX11::ClearReleasedShaders()
			{
				m_loadedShaders.Cleanup();
			}
			COM_Ptr<ID3DBlob> VertexShader_DX11::CompileVertexShader(Graphics_DX11& graphics, std::string& shaderCode)
			{
				const char* target = nullptr;
				switch (graphics.getFeatureLevel())
				{
				case D3D_FEATURE_LEVEL_10_0:
					target = "vs_4_0";
					break;
				case D3D_FEATURE_LEVEL_10_1:
					target = "vs_4_1";
					break;
				case D3D_FEATURE_LEVEL_11_0:
				case D3D_FEATURE_LEVEL_11_1:
					target = "vs_5_0";
					break;
				default:
					target = "vs_4_0";
				}
				return CompileShader(shaderCode, "main", target, L"shadererror.txt");
			}
			void VertexShader_DX11::CreateVertexShader(Graphics_DX11& graphics, ID3DBlob* shaderBuffer)
			{
				HRESULT hr = graphics.getDevice()->CreateVertexShader(shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), nullptr, &m_vertexShader);
				if (FAILED(hr))
					throw std::exception("Failed to create vertex shader.");
			}
			void VertexShader_DX11::CreateInputLayout(Graphics_DX11& graphics, ID3DBlob* shaderBuffer)
			{
				int counter = 0;
				D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[7];
				if (ModelType::HasPositions(m_modelType))
					inputLayoutDesc[counter++] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				if (ModelType::HasTexcoords(m_modelType))
					inputLayoutDesc[counter++] = { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				if (ModelType::HasNormals(m_modelType))
					inputLayoutDesc[counter++] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				if (ModelType::HasTangents(m_modelType))
					inputLayoutDesc[counter++] = { "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				if (ModelType::HasBones(m_modelType))
				{
					inputLayoutDesc[counter++] = { "BONEWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
					inputLayoutDesc[counter++] = { "BONEINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
				}

				HRESULT hr = graphics.getDevice()->CreateInputLayout(inputLayoutDesc, counter,
					shaderBuffer->GetBufferPointer(), shaderBuffer->GetBufferSize(), &m_inputLayout);
				if (FAILED(hr))
					throw std::exception("Failed to create input layout.");
			}
			VertexShader_DX11::VertexShader_DX11(Graphics_DX11& graphics, LPCWSTR shaderFileName, unsigned modelType)
			{
				m_modelType = modelType;
				COM_Ptr<ID3DBlob> shaderBuffer = LoadShaderCode(shaderFileName);

				CreateVertexShader(graphics, shaderBuffer);
				CreateInputLayout(graphics, shaderBuffer);
			}
			VertexShader_DX11::VertexShader_DX11(Graphics_DX11& graphics, unsigned modelType)
			{
				m_modelType = modelType;
				std::string shaderCode;
				shaderCode.reserve(1024);
				shaderCode += "cbuffer PerFrame{matrix cameraMatrix;};cbuffer PerObject{matrix worldMatrix;};struct VertexInputType{";
				if (ModelType::HasPositions(modelType))
					shaderCode += "float3 position:POSITION;";
				if (ModelType::HasTexcoords(modelType))
					shaderCode += "float2 tex:TEXCOORD;";
				if (ModelType::HasNormals(modelType))
					shaderCode += "float3 normal:NORMAL;";
				if (ModelType::HasTangents(modelType))
					shaderCode += "float3 tangent:TANGENT;";
				shaderCode += "};struct PixelInputType{";
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
				shaderCode += "};PixelInputType main(VertexInputType input){PixelInputType output;";
				if (ModelType::HasPositions(modelType))
					shaderCode += "output.position=mul(float4(input.position,1),worldMatrix);";
				if (ModelType::HasNormals(modelType))
					shaderCode += "output.pos=output.position.xyz;";
				if (ModelType::HasPositions(modelType))
					shaderCode += "output.position=mul(output.position,cameraMatrix);";
				if (ModelType::HasTexcoords(modelType))
					shaderCode += "output.tex=input.tex;";
				if (ModelType::HasNormals(modelType))
					shaderCode += "output.normal=mul(input.normal,(float3x3)worldMatrix);";
				if (ModelType::HasTangents(modelType))
					shaderCode += "output.tangent=mul(input.tangent,(float3x3)worldMatrix);";
				shaderCode += "return output;}";

				COM_Ptr<ID3DBlob> shaderBuffer = CompileVertexShader(graphics, shaderCode);
				CreateVertexShader(graphics, shaderBuffer);
				CreateInputLayout(graphics, shaderBuffer);
			}
			VertexShader_DX11::VertexShader_DX11(Graphics_DX11& graphics, const char* shaderCode, unsigned modelType)
			{
				m_modelType = modelType;
				std::string code(shaderCode);
				COM_Ptr<ID3DBlob> shaderBuffer = CompileVertexShader(graphics, code);
				CreateVertexShader(graphics, shaderBuffer);
				CreateInputLayout(graphics, shaderBuffer);
			}
			VertexShader_DX11::~VertexShader_DX11()
			{
				ClearReleasedShaders();
			}
			VertexShader_DX11::P VertexShader_DX11::CreateP(Graphics_DX11& graphics, LPCWSTR shaderFileName, unsigned modelType)
			{
				VertexShader_DX11::P vs = m_loadedShaders.Find(shaderFileName);
				if (vs == nullptr)
				{
					vs = std::make_shared<VertexShader_DX11>(graphics, shaderFileName, modelType);
					m_loadedShaders.Add(shaderFileName, vs);
				}
				return vs;
			}
			VertexShader_DX11::U VertexShader_DX11::CreateU(Graphics_DX11& graphics, LPCWSTR shaderFileName, unsigned modelType)
			{
				return std::make_unique<VertexShader_DX11>(graphics, shaderFileName, modelType);
			}
			VertexShader_DX11::P VertexShader_DX11::CreateP(Graphics_DX11& graphics, unsigned modelType)
			{
				return std::make_shared<VertexShader_DX11>(graphics, modelType);
			}
			VertexShader_DX11::U VertexShader_DX11::CreateU(Graphics_DX11& graphics, unsigned modelType)
			{
				return std::make_unique<VertexShader_DX11>(graphics, modelType);
			}
			VertexShader_DX11::P VertexShader_DX11::CreatePFromString(Graphics_DX11& graphics, const char* shaderCode, unsigned modelType)
			{
				return std::make_shared<VertexShader_DX11>(graphics, shaderCode, modelType);
			}
			VertexShader_DX11::U VertexShader_DX11::CreateUFromString(Graphics_DX11& graphics, const char* shaderCode, unsigned modelType)
			{
				return std::make_unique<VertexShader_DX11>(graphics, shaderCode, modelType);
			}
			void VertexShader_DX11::SetShaderToRender(Graphics& graphics)
			{
				ID3D11DeviceContext *context = ((Graphics_DX11&)graphics).getContext();
				context->IASetInputLayout(m_inputLayout);
				context->VSSetShader(m_vertexShader, nullptr, 0);
			}
		}
	}
}