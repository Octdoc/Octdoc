#include "modelloader.h"
#include"omdloader.h"

namespace octdoc
{
	namespace hlp
	{
		ModelData::MaterialData::Texture::Texture() :
			filename(),
			width(0),
			height(0),
			data(),
			loaded(false) {}
		ModelData::MaterialData::Texture::Texture(const wchar_t* str) :
			filename(str),
			width(0),
			height(0),
			data(),
			loaded(false) {}
		void ModelData::MaterialData::Texture::Clear()
		{
			filename.clear();
			width = 0;
			height = 0;
			data.clear();
			loaded = false;
		}
		void ModelData::MaterialData::Clear()
		{
			texture.Clear();
			normalmap.Clear();
			materialType = 0;
		}
		void ModelData::Clear()
		{
			modelType = 0;
			vertexSizeInBytes = 0;
			vertexCount = 0;
			vertices.clear();
			indices.clear();
			vertexGroups.clear();
			materials.clear();

			boundingVolumeType = 0;
			bvPosition = 0;
			bvCuboidSize = 0;
			bvSphereRadius = 0;
			hitboxVertices.clear();
			hitboxIndices.clear();
		}

		void ModelLoader::Create(void* vertices, unsigned vertexCount, unsigned indices[], unsigned indexCount, unsigned modelType)
		{
			m_data.vertexCount = vertexCount;
			m_data.modelType = modelType;
			m_data.vertexSizeInBytes = gfx::ModelType::VertexSizeInBytes(m_data.modelType);
			m_data.vertices.resize(m_data.vertexCount * m_data.vertexSizeInBytes / 4);
			memcpy(m_data.vertices.data(), vertices, m_data.vertexCount * m_data.vertexSizeInBytes);
			m_data.indices.resize(indexCount);
			memcpy(m_data.indices.data(), indices, indexCount * 4);
			m_data.vertexGroups.push_back(gfx::VertexGroup({ 0, unsigned(m_data.indices.size()), 0 }));
			m_data.materials.push_back(ModelData::MaterialData());
		}

		void ModelLoader::StoreFilePath(const wchar_t* path)
		{
			DisassemblePath(path, &m_folderName, &m_bareFileName, &m_extension);
		}
		ModelLoader::ModelLoader() :
			m_data(), m_folderName(), m_bareFileName() {}
		ModelLoader::ModelLoader(const wchar_t* filename) :
			m_data(), m_folderName(), m_bareFileName()
		{
			LoadModel(filename);
		}
		void ModelLoader::LoadModel(const wchar_t* filename)
		{
			Clear();
			StoreFilePath(filename);
			if (m_extension == L".omd")
				OMDLoader(m_data).LoadOMD(filename);
			else
				throw std::exception(("Could not load file: " + ToString(filename) + "\nFormat not supported").c_str());
		}
		void ModelLoader::Clear()
		{
			m_data.Clear();
			m_folderName.clear();
			m_bareFileName.clear();
			m_extension.clear();
		}
		void ModelLoader::CreateSphere(mth::float3 centre, float radius, int longitudes, int latitudes, unsigned modelType)
		{
			std::vector<mth::float3> vertices(longitudes * latitudes + 2);
			std::vector<unsigned> indices(3 * longitudes + 6 * (latitudes - 1) * longitudes + 3 * longitudes);
			float lonStep = mth::PI * 2.0f / longitudes;
			float latStep = mth::PI / (latitudes + 1);
			int counter = 0;
			vertices[counter++] = mth::float3(0.0f, radius, 0.0f);
			for (int i = 0; i < latitudes; i++)
			{
				float lat = (i + 1) * latStep;
				for (int j = 0; j < longitudes; j++)
				{
					float lon = j * lonStep;
					vertices[counter++] = mth::float3(sinf(lon) * sinf(lat), cosf(lat), cosf(lon) * sinf(lat)) * radius;
				}
			}
			vertices[counter++] = mth::float3(0.0f, -radius, 0.0f);
			counter = 0;
			for (int lon = 0; lon < longitudes; lon++)
			{
				indices[counter++] = 0;
				indices[counter++] = lon % longitudes + 1;
				indices[counter++] = (lon + 1) % longitudes + 1;
			}
			for (int lat = 0; lat < latitudes - 1; lat++)
			{
				for (int lon = 0; lon < longitudes; lon++)
				{
					indices[counter++] = lat * longitudes + 1 + lon % longitudes;
					indices[counter++] = lat * longitudes + 1 + (lon + 1) % longitudes + longitudes;
					indices[counter++] = lat * longitudes + 1 + (lon + 1) % longitudes;
					indices[counter++] = lat * longitudes + 1 + lon % longitudes;
					indices[counter++] = lat * longitudes + 1 + lon % longitudes + longitudes;
					indices[counter++] = lat * longitudes + 1 + (lon + 1) % longitudes + longitudes;
				}
			}
			int last = int(vertices.size() - 1);
			for (int lon = 0; lon < longitudes; lon++)
			{
				indices[counter++] = last;
				indices[counter++] = (latitudes - 1) * longitudes + (lon + 1) % longitudes + 1;
				indices[counter++] = (latitudes - 1) * longitudes + lon % longitudes + 1;
			}

			Create(vertices.data(), vertices.size(), indices.data(), indices.size(), gfx::ModelType::P);
			ChangeModelType(modelType);
		}
		void ModelLoader::CreateCube(mth::float3 p, mth::float3 s, unsigned modelType)
		{
			Clear();
			float vertices[] = {
				p.x + s.x, p.y, p.z + s.z,			1.0f, 1.0f,
				p.x + s.x, p.y, p.z,				1.0f, 0.0f,
				p.x, p.y, p.z,						0.0f, 0.0f,
				p.x, p.y, p.z + s.z,				0.0f, 1.0f,
				p.x + s.x, p.y + s.y, p.z + s.z,	1.0f, 0.0f,
				p.x, p.y + s.y, p.z + s.z,			0.0f, 0.0f,
				p.x, p.y + s.y, p.z,				0.0f, 1.0f,
				p.x + s.x, p.y + s.y, p.z,			1.0f, 1.0f,
				p.x + s.x, p.y, p.z + s.z,			1.0f, 1.0f,
				p.x + s.x, p.y + s.y, p.z + s.z,	1.0f, 0.0f,
				p.x + s.x, p.y + s.y, p.z,			0.0f, 0.0f,
				p.x + s.x, p.y, p.z,				0.0f, 1.0f,
				p.x + s.x, p.y, p.z,				1.0f, 1.0f,
				p.x + s.x, p.y + s.y, p.z,			1.0f, 0.0f,
				p.x, p.y + s.y, p.z,				0.0f, 0.0f,
				p.x, p.y, p.z,						0.0f, 1.0f,
				p.x, p.y, p.z,						1.0f, 1.0f,
				p.x, p.y + s.y, p.z,				1.0f, 0.0f,
				p.x, p.y + s.y, p.z + s.z,			0.0f, 0.0f,
				p.x, p.y, p.z + s.z,				0.0f, 1.0f,
				p.x + s.x, p.y + s.y, p.z + s.z,	0.0f, 0.0f,
				p.x + s.x, p.y, p.z + s.z,			0.0f, 1.0f,
				p.x, p.y, p.z + s.z,				1.0f, 1.0f,
				p.x, p.y + s.y, p.z + s.z,			1.0f, 0.0f
			};
			unsigned indices[] = {
				0, 2, 1, 0, 3, 2, 4, 6, 5, 4, 7, 6, 8, 10, 9, 8, 11, 10, 12, 14, 13, 12, 15, 14, 16, 18, 17, 16, 19, 18, 20, 22, 21, 20, 23, 22
			};
			Create(vertices, 24, indices, 36, gfx::ModelType::PT);
			ChangeModelType(modelType);
		}
		void ModelLoader::CreateQuad(mth::float2 xzposition, mth::float2 xzsize, unsigned modelType)
		{
			float vertices[] = {
			xzposition.x + xzsize.x, 0.0f, xzposition.y,				1.0f, 1.0f,
			xzposition.x + xzsize.x, 0.0f, xzposition.y + xzsize.y,		1.0f, 0.0f,
			xzposition.x, 0.0f, xzposition.y + xzsize.y,				0.0f, 0.0f,
			xzposition.x, 0.0f, xzposition.y,							0.0f, 1.0f
			};
			unsigned indices[] = { 0, 2, 1, 0, 3, 2 };
			Create(vertices, 4, indices, 6, gfx::ModelType::PT);
			ChangeModelType(modelType);
		}
		void ModelLoader::CreateFullScreenQuad()
		{
			float vertices[] = {
			1.0f, 0.0f, 1.0f,		1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,		1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.0f, 0.0f,
			0.0f, 0.0f, 1.0f,		0.0f, 1.0f
			};
			unsigned indices[] = { 0, 2, 1, 0, 3, 2 };
			Create(vertices, 4, indices, 6, gfx::ModelType::PT);
		}
		void ModelLoader::FlipInsideOut()
		{
			for (size_t i = 0; i < m_data.indices.size(); i += 3)
			{
				unsigned tmp = m_data.indices[i + 1];
				m_data.indices[i + 1] = m_data.indices[i + 2];
				m_data.indices[i + 2] = tmp;
			}
			unsigned vertexSize_ve = m_data.vertexSizeInBytes / 4;
			if (gfx::ModelType::HasNormals(m_data.modelType))
			{
				unsigned offset = gfx::ModelType::NormalOffset(m_data.modelType);
				for (unsigned i = 0; i < getVertexCount(); i++)
				{
					m_data.vertices[i * vertexSize_ve + offset + 0].f *= -1.0f;
					m_data.vertices[i * vertexSize_ve + offset + 1].f *= -1.0f;
					m_data.vertices[i * vertexSize_ve + offset + 2].f *= -1.0f;
				}
			}
			if (gfx::ModelType::HasTangents(m_data.modelType))
			{
				unsigned offset = gfx::ModelType::TangentOffset(m_data.modelType);
				for (unsigned i = 0; i < getVertexCount(); i++)
				{
					m_data.vertices[i * vertexSize_ve + offset + 0].f *= -1.0f;
					m_data.vertices[i * vertexSize_ve + offset + 1].f *= -1.0f;
					m_data.vertices[i * vertexSize_ve + offset + 2].f *= -1.0f;
				}
			}
		}
#pragma region Change model type
		void ModelLoader::CopyPositions(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter)
		{
			if (gfx::ModelType::HasPositions(newModelType))
			{
				if (gfx::ModelType::HasPositions(m_data.modelType))
				{
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
				}
				else
				{
					newVertices[newCounter++] = 0.0f;
					newVertices[newCounter++] = 0.0f;
					newVertices[newCounter++] = 0.0f;
				}
			}
			else if (gfx::ModelType::HasPositions(m_data.modelType))
				oldCounter += 3;
		}
		void ModelLoader::CopyTexCoords(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter)
		{
			if (gfx::ModelType::HasTexcoords(newModelType))
			{
				if (gfx::ModelType::HasTexcoords(m_data.modelType))
				{
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
				}
				else
				{
					newCounter += 2;	//UV mapping will be applied; better than nothing
				}
			}
			else if (gfx::ModelType::HasTexcoords(m_data.modelType))
				oldCounter += 2;
		}
		void ModelLoader::CopyNormals(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter)
		{
			if (gfx::ModelType::HasNormals(newModelType))
			{
				if (gfx::ModelType::HasNormals(m_data.modelType))
				{
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
				}
				else
				{
					newCounter += 3;	//normals will be calculated
				}
			}
			else if (gfx::ModelType::HasNormals(m_data.modelType))
				oldCounter += 3;
		}
		void ModelLoader::CopyTangentBinormals(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter)
		{
			if (gfx::ModelType::HasTangents(newModelType))
			{
				if (gfx::ModelType::HasTangents(m_data.modelType))
				{
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
				}
				else
				{
					newCounter += 3;	//normals will be calculated
				}
			}
			else if (gfx::ModelType::HasTangents(m_data.modelType))
				oldCounter += 3;
		}
		void ModelLoader::CopyBones(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter)
		{
			if (gfx::ModelType::HasBones(newModelType))
			{
				if (gfx::ModelType::HasBones(m_data.modelType))
				{
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
					newVertices[newCounter++] = m_data.vertices[oldCounter++];
				}
				else
				{
					newVertices[newCounter++] = 1.0f;
					newVertices[newCounter++] = 0.0f;
					newVertices[newCounter++] = 0.0f;
					newVertices[newCounter++] = 0.0f;
					newVertices[newCounter++] = 0u;
					newVertices[newCounter++] = 0u;
					newVertices[newCounter++] = 0u;
					newVertices[newCounter++] = 0u;
				}
			}
			else if (gfx::ModelType::HasBones(m_data.modelType))
				oldCounter += 8;
		}
		void ModelLoader::ChangeModelType(unsigned newModelType)
		{
			if (newModelType != m_data.modelType)
			{
				unsigned newVertexSize_ve = gfx::ModelType::VertexSizeInVertexElements(newModelType);
				std::vector<gfx::VertexElement> newVertices(m_data.vertexCount * newVertexSize_ve);
				int oldCounter = 0, newCounter = 0;
				for (int i = 0; i < m_data.vertexCount; i++)
				{
					CopyPositions(newVertices, newModelType, newCounter, oldCounter);
					CopyTexCoords(newVertices, newModelType, newCounter, oldCounter);
					CopyNormals(newVertices, newModelType, newCounter, oldCounter);
					CopyTangentBinormals(newVertices, newModelType, newCounter, oldCounter);
					CopyBones(newVertices, newModelType, newCounter, oldCounter);
				}
				unsigned oldModelType = m_data.modelType;
				m_data.vertices.swap(newVertices);
				m_data.modelType = newModelType;
				m_data.vertexSizeInBytes = newVertexSize_ve * 4;
				if (gfx::ModelType::HasTexcoords(newModelType) && !gfx::ModelType::HasTexcoords(oldModelType))
					UVMap();
				if (gfx::ModelType::HasNormals(newModelType) && !gfx::ModelType::HasNormals(oldModelType))
					CalculateNormals();
				if (gfx::ModelType::HasTangents(newModelType) && !gfx::ModelType::HasTangents(oldModelType))
					CalculateTangents();
			}
		}
#pragma endregion
		void ModelLoader::Transform(mth::float4x4 transform)
		{
			mth::float3x3 surfaceTransform = mth::float3x3(transform).Transposed().Inverse();
			unsigned vertexSize = m_data.vertexSizeInBytes / 4;
			mth::float3 v;
			if (gfx::ModelType::HasPositions(m_data.modelType))
			{
				unsigned offset = gfx::ModelType::PositionOffset(m_data.modelType);
				for (unsigned i = 0; i < getVertexCount(); i++)
				{
					v = mth::Transform(transform, mth::float3(
						m_data.vertices[i * vertexSize + offset + 0].f,
						m_data.vertices[i * vertexSize + offset + 1].f,
						m_data.vertices[i * vertexSize + offset + 2].f
					));
					m_data.vertices[i * vertexSize + offset + 0] = v.x;
					m_data.vertices[i * vertexSize + offset + 1] = v.y;
					m_data.vertices[i * vertexSize + offset + 2] = v.z;
				}
			}
			if (gfx::ModelType::HasNormals(m_data.modelType))
			{
				unsigned offset = gfx::ModelType::NormalOffset(m_data.modelType);
				for (unsigned i = 0; i < getVertexCount(); i++)
				{
					v = surfaceTransform * mth::float3(
						m_data.vertices[i * vertexSize + offset + 0].f,
						m_data.vertices[i * vertexSize + offset + 1].f,
						m_data.vertices[i * vertexSize + offset + 2].f
					);
					v.Normalize();
					m_data.vertices[i * vertexSize + offset + 0] = v.x;
					m_data.vertices[i * vertexSize + offset + 1] = v.y;
					m_data.vertices[i * vertexSize + offset + 2] = v.z;
				}
			}
			if (gfx::ModelType::HasTangents(m_data.modelType))
			{
				unsigned offset = gfx::ModelType::TangentOffset(m_data.modelType);
				for (unsigned i = 0; i < getVertexCount(); i++)
				{
					v = surfaceTransform * mth::float3(
						m_data.vertices[i * vertexSize + offset + 0].f,
						m_data.vertices[i * vertexSize + offset + 1].f,
						m_data.vertices[i * vertexSize + offset + 2].f
					);
					v.Normalize();
					m_data.vertices[i * vertexSize + offset + 0] = v.x;
					m_data.vertices[i * vertexSize + offset + 1] = v.y;
					m_data.vertices[i * vertexSize + offset + 2] = v.z;
				}
			}
			if (HasHitbox())
			{
				mth::double4x4 trmat = (mth::double4x4)transform;
				for (mth::double3& v : m_data.hitboxVertices)
					v = mth::Transform(trmat, v);
			}
		}
		void ModelLoader::CalculateNormals()
		{
			if (gfx::ModelType::HasNormals(m_data.modelType) && gfx::ModelType::HasPositions(m_data.modelType))
			{
				unsigned vs_ve = m_data.vertexSizeInBytes / 4;
				unsigned normalOffset = gfx::ModelType::NormalOffset(m_data.modelType);
				for (unsigned i = 0; i < m_data.vertexCount; i++)
				{
					m_data.vertices[i * vs_ve + normalOffset + 0] = 0.0f;
					m_data.vertices[i * vs_ve + normalOffset + 1] = 0.0f;
					m_data.vertices[i * vs_ve + normalOffset + 2] = 0.0f;
				}
				for (unsigned i = 0; i < m_data.indices.size(); i += 3)
				{
					mth::float3 v1((float*)& m_data.vertices[m_data.indices[i + 0] * vs_ve]);
					mth::float3 v2((float*)& m_data.vertices[m_data.indices[i + 1] * vs_ve]);
					mth::float3 v3((float*)& m_data.vertices[m_data.indices[i + 2] * vs_ve]);
					mth::float3 n = (v1 - v2).Cross(v1 - v3);
					*((mth::float3*) & m_data.vertices[m_data.indices[i + 0] * vs_ve + normalOffset]) += n;
					*((mth::float3*) & m_data.vertices[m_data.indices[i + 1] * vs_ve + normalOffset]) += n;
					*((mth::float3*) & m_data.vertices[m_data.indices[i + 2] * vs_ve + normalOffset]) += n;
				}
				for (unsigned i = 0; i < m_data.vertexCount; i++)
				{
					((mth::float3*) & m_data.vertices[i * vs_ve + normalOffset])->Normalize();
				}
			}
		}
		void ModelLoader::UVMap()
		{
			if (gfx::ModelType::HasTexcoords(m_data.modelType) && gfx::ModelType::HasPositions(m_data.modelType))
			{
				unsigned vs_ve = m_data.vertexSizeInBytes / 4;
				unsigned offset = gfx::ModelType::TexCoordOffset(m_data.modelType);
				for (unsigned i = 0; i < m_data.vertexCount; i++)
				{
					mth::float3 p = ((mth::float3*) & m_data.vertices[i * vs_ve])->Normalized();
					if (mth::isNear(fabsf(p.y), 1.0f))
					{
						m_data.vertices[i * vs_ve + offset + 0] = 0.5f;
						m_data.vertices[i * vs_ve + offset + 1] = p.y < 0.0f ? 1.0f : 0.0f;
					}
					else
					{
						m_data.vertices[i * vs_ve + offset + 0].f = (atan2f(p.x, -p.z) + mth::PI) / (mth::PI * 2.0f);
						m_data.vertices[i * vs_ve + offset + 1].f = acosf(p.y) / mth::PI;
					}
				}
			}
		}
		void ModelLoader::CalculateTangents()
		{
			if (gfx::ModelType::HasPositions(m_data.modelType) && gfx::ModelType::HasTexcoords(m_data.modelType) && gfx::ModelType::HasTangents(m_data.modelType))
			{
				unsigned vs_ve = m_data.vertexSizeInBytes / 4;
				unsigned texcoordOffset = gfx::ModelType::TexCoordOffset(m_data.modelType);
				unsigned tangentOffset = gfx::ModelType::TangentOffset(m_data.modelType);
				for (unsigned i = 0; i < m_data.vertexCount; i++)
				{
					m_data.vertices[i * vs_ve + tangentOffset + 0] = 0.0f;
					m_data.vertices[i * vs_ve + tangentOffset + 1] = 0.0f;
					m_data.vertices[i * vs_ve + tangentOffset + 2] = 0.0f;
				}
				for (unsigned i = 0; i < m_data.indices.size(); i += 3)
				{
					mth::float3 vertex((float*)& m_data.vertices[m_data.indices[i + 0] * vs_ve]);
					mth::float2 texcoord((float*)& m_data.vertices[m_data.indices[i + 0] * vs_ve + texcoordOffset]);
					mth::float3 v1 = mth::float3((float*)& m_data.vertices[m_data.indices[i + 1] * vs_ve]) - vertex;
					mth::float3 v2 = mth::float3((float*)& m_data.vertices[m_data.indices[i + 2] * vs_ve]) - vertex;
					mth::float2 t1 = mth::float2((float*)& m_data.vertices[m_data.indices[i + 1] * vs_ve + texcoordOffset]) - texcoord;
					mth::float2 t2 = mth::float2((float*)& m_data.vertices[m_data.indices[i + 2] * vs_ve + texcoordOffset]) - texcoord;
					float den = 1.0f / (t1.x * t2.y - t1.y * t2.x);
					*((mth::float3*) & m_data.vertices[m_data.indices[i + 0] * vs_ve + tangentOffset]) += (t1.x * v1 - t1.y * v2) * den;
					*((mth::float3*) & m_data.vertices[m_data.indices[i + 1] * vs_ve + tangentOffset]) += (t1.x * v1 - t1.y * v2) * den;
					*((mth::float3*) & m_data.vertices[m_data.indices[i + 2] * vs_ve + tangentOffset]) += (t1.x * v1 - t1.y * v2) * den;


				}
				for (unsigned i = 0; i < m_data.vertexCount; i++)
					((mth::float3*) & m_data.vertices[i * vs_ve + tangentOffset])->Normalize();
			}
		}
		void ModelLoader::Triangularize()
		{
			unsigned vertexSize_byte = m_data.vertexSizeInBytes;
			unsigned vertexSize_ve = vertexSize_byte / 4;
			unsigned vertexCount = m_data.indices.size();
			std::vector<gfx::VertexElement> vertices(vertexCount * vertexSize_ve);
			for (unsigned i = 0; i < vertexCount; i++)
				memcpy(&vertices[i * vertexSize_ve], &m_data.vertices[m_data.indices[i] * vertexSize_ve], vertexSize_byte);
			m_data.vertices.swap(vertices);
			m_data.vertexCount = vertexCount;
			for (unsigned i = 0; i < vertexCount; i++)
				m_data.indices[i] = i;
		}
		void ModelLoader::MakeHitboxFromVertices()
		{
			unsigned vs_ve = m_data.vertexSizeInBytes / 4;
			m_data.hitboxVertices.resize(m_data.vertexCount);
			mth::double3 minpos, maxpos;
			minpos.x = maxpos.x = m_data.vertices[0].f;
			minpos.y = maxpos.y = m_data.vertices[1].f;
			minpos.z = maxpos.z = m_data.vertices[2].f;
			for (unsigned i = 0; i < m_data.vertexCount; i++)
			{
				mth::double3& v = m_data.hitboxVertices[i];
				v.x = m_data.vertices[vs_ve * i + 0].f;
				v.y = m_data.vertices[vs_ve * i + 1].f;
				v.z = m_data.vertices[vs_ve * i + 2].f;
				if (minpos.x > v.x) minpos.x = v.x;
				if (minpos.y > v.y) minpos.y = v.y;
				if (minpos.z > v.z) minpos.z = v.z;
				if (maxpos.x < v.x) maxpos.x = v.x;
				if (maxpos.y < v.y) maxpos.y = v.y;
				if (maxpos.z < v.z) maxpos.z = v.z;
			}
			m_data.hitboxIndices = m_data.indices;
			m_data.boundingVolumeType = physx::BoundingVolume::CUBOID;
			m_data.bvPosition = minpos;
			m_data.bvCuboidSize = maxpos - minpos;
		}
		void ModelLoader::MakeVerticesFromHitbox()
		{
			m_data.vertexCount = m_data.hitboxVertices.size();
			m_data.vertices.resize(m_data.vertexCount * 3);
			m_data.modelType = gfx::ModelType::P;
			m_data.vertexSizeInBytes = gfx::ModelType::VertexSizeInBytes(m_data.modelType);
			for (unsigned i = 0; i < m_data.vertexCount; i++)
			{
				m_data.vertices[3 * i + 0].f = m_data.hitboxVertices[i].x;
				m_data.vertices[3 * i + 1].f = m_data.hitboxVertices[i].y;
				m_data.vertices[3 * i + 2].f = m_data.hitboxVertices[i].z;
			}
			m_data.indices = m_data.hitboxIndices;
			m_data.vertexGroups.resize(1);
			m_data.vertexGroups[0].startIndex = 0;
			m_data.vertexGroups[0].indexCount = m_data.indices.size();
			m_data.vertexGroups[0].materialIndex = 0;
			m_data.materials.resize(1);
			m_data.materials[0].Clear();
		}
		bool ModelLoader::HasHitbox()
		{
			return m_data.boundingVolumeType != physx::BoundingVolume::NO_TYPE;
		}
		void ModelLoader::SwapHitboxes(ModelLoader& other)
		{
			std::swap(m_data.boundingVolumeType, other.m_data.boundingVolumeType);
			std::swap(m_data.bvPosition, other.m_data.bvPosition);
			std::swap(m_data.bvCuboidSize, other.m_data.bvCuboidSize);
			std::swap(m_data.bvSphereRadius, other.m_data.bvSphereRadius);
			std::swap(m_data.hitboxVertices, other.m_data.hitboxVertices);
			std::swap(m_data.hitboxIndices, other.m_data.hitboxIndices);
		}
	}
}