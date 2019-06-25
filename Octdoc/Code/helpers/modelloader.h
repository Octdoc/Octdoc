#pragma once

#include "graphics/modeltype.h"
#include "geometry3d.h"
#include "physics/boundingvolume.h"
#include <vector>
#include <string>
#include <fstream>

namespace octdoc
{
	namespace hlp
	{
		struct ModelData
		{
			struct MaterialData
			{
				struct Texture
				{
					std::wstring filename;
					int width;
					int height;

					/* pixel (x,y) can be accessed: data[(x+y*width)*4+component]
					where component is: 0 for red, 1 for green, 2 for blue, 3 for alpha */
					std::vector<unsigned char> data;
					bool loaded;	//if true, texture can be created from data, if false, texture can be loaded from <filename> file

					Texture();
					Texture(const wchar_t* str);
					void Clear();
				};
				Texture texture;
				Texture normalmap;
				unsigned materialType;

				void Clear();
			};

			unsigned modelType;
			unsigned vertexSizeInBytes;
			unsigned vertexCount;
			std::vector<gfx::VertexElement> vertices;
			std::vector<unsigned> indices;
			std::vector<gfx::VertexGroup> vertexGroups;
			std::vector<MaterialData> materials;

			unsigned boundingVolumeType;
			mth::float3 bvPosition;
			mth::float3 bvCuboidSize;
			float bvSphereRadius;
			std::vector<mth::float3> hitboxVertices;
			std::vector<unsigned> hitboxIndices;
			inline bool isHitboxIndexed() { return hitboxIndices.size() != 0; }

			void Clear();
		};

		class ModelLoader
		{
		private:
			ModelData m_data;
			std::wstring m_folderName;
			std::wstring m_bareFileName;
			std::wstring m_extension;

		private:
			void Create(void* vertices, unsigned vertexCount, unsigned indices[], unsigned indexCount, unsigned modelType);
			void CopyPositions(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter);
			void CopyTexCoords(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter);
			void CopyNormals(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter);
			void CopyTangentBinormals(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter);
			void CopyBones(std::vector<gfx::VertexElement>& newVertices, unsigned newModelType, int& newCounter, int& oldCounter);

		protected:
			void StoreFilePath(const wchar_t* path);

		public:
			ModelLoader();
			ModelLoader(const wchar_t* filename);
			void LoadModel(const wchar_t* filename);
			void Clear();

			void CreateSphere(mth::float3 centre, float radius, int longitudes, int latitudes, unsigned modelType);
			void CreateCube(mth::float3 position, mth::float3 size, unsigned modelType);
			void CreateQuad(mth::float2 xzposition, mth::float2 xzsize, unsigned modelType);
			void CreateFullScreenQuad();

			void FlipInsideOut();
			void ChangeModelType(unsigned newModelType);
			void Transform(mth::float4x4 transform);
			void CalculateNormals();
			void UVMap();
			void CalculateTangents();
			void Triangularize();

			void MakeHitboxFromVertices();
			void MakeVerticesFromHitbox();
			bool HasHitbox();
			void SwapHitboxes(ModelLoader& other);

			inline std::wstring& getFolderName() { return m_folderName; }
			inline gfx::VertexElement* getVertices() { return m_data.vertices.data(); }
			inline unsigned getVertexCount() { return m_data.vertexCount; }
			inline unsigned* getIndices() { return m_data.indices.data(); }
			inline unsigned getIndexCount() { return (unsigned)m_data.indices.size(); }
			inline unsigned getModelType() { return m_data.modelType; }
			inline unsigned getVertexSizeInBytes() { return m_data.vertexSizeInBytes; }
			inline gfx::VertexGroup& getVertexGroup(unsigned index) { return m_data.vertexGroups[index]; }
			inline unsigned getVertexGroupCount() { return (unsigned)m_data.vertexGroups.size(); }
			inline unsigned getMaterialCount() { return (unsigned)m_data.materials.size(); }
			inline ModelData::MaterialData::Texture& getTexture(unsigned index) { return m_data.materials[index].texture; }
			inline ModelData::MaterialData::Texture& getNormalmap(unsigned index) { return m_data.materials[index].normalmap; }
			inline unsigned getMaterialType(unsigned index) { return m_data.materials[index].materialType; }

			inline unsigned getBoundingVolumePrimitive() { return m_data.boundingVolumeType; }
			inline mth::float3 getBoundingVolumePosition() { return m_data.bvPosition; }
			inline mth::float3 getBoundingVolumeCubiodSize() { return m_data.bvCuboidSize; }
			inline float getBoundingVolumeSphereRadius() { return m_data.bvSphereRadius; }
			inline std::vector<mth::float3>& getHitboxMesh() { return m_data.hitboxVertices; }
			inline std::vector<unsigned>& getHitboxIndices() { return m_data.hitboxIndices; }
			inline bool isHitboxIndexed() { return m_data.isHitboxIndexed(); }
		};
	}
}