#include "omdloader.h"

namespace octdoc
{
	namespace hlp
	{
		OMDLoader::OMDHeader::OMDHeader() :
			fileFormat(0),
			extension{ 0,0,0 },
			modelType(0),
			vertexCount(0),
			indexCount(0),
			groupCount(0),
			materialCount(0),
			boundingVolumePrimitive(0),
			hitboxTriangleCount(0),
			boneCount(0),
			animationCount(0){}

		OMDLoader::OMDLoader(ModelData& data) : m_data(data) {}
		void OMDLoader::ThrowCurruptedFileException()
		{
			throw std::exception(ToString((L"Corrupted file: " + m_filename).c_str()).c_str());
		}
		void OMDLoader::LoadOMD(const wchar_t* filename)
		{
			m_filename = filename;
			DisassemblePath(filename, &m_foldername, nullptr, nullptr);
			std::ifstream infile(m_filename);
			char ch;
			infile.read(&ch, 1);
			infile.close();
			if (ch == 't' || ch == 'T')
				LoadOMDText();
			else if (ch == 'b' || ch == 'B')
				LoadOMDBinary();
			else
				ThrowCurruptedFileException();
		}

#pragma region Load OMD text

		void OMDLoader::LoadOMDText()
		{
			std::wifstream infile(m_filename);
			ReadHeaderText(infile);
			ReadVerticesText(infile);
			ReadIndicesText(infile);
			ReadGroupsText(infile);
			ReadMaterialsText(infile);
			ReadHitboxText(infile);
			ReadBonesText(infile);
			ReadAnimationsText(infile);
		}
		void OMDLoader::ReadHeaderText(std::wifstream& infile)
		{
			std::wstring buffer;
			infile >> buffer;
			if (!((buffer[0] & 32) == 't') && ((buffer[1] & 32) == 'o') && ((buffer[2] & 32) == 'm') && ((buffer[3] & 32) == 'd'))
				ThrowCurruptedFileException();
			if (!ScanText(infile, L"Model type:"))
				ThrowCurruptedFileException();
			infile >> m_header.modelType;
			if (!ScanText(infile, L"Vertex count:"))
				ThrowCurruptedFileException();
			infile >> m_header.vertexCount;
			if (!ScanText(infile, L"Index count:"))
				ThrowCurruptedFileException();
			infile >> m_header.indexCount;
			if (!ScanText(infile, L"Group count:"))
				ThrowCurruptedFileException();
			infile >> m_header.groupCount;
			if (!ScanText(infile, L"Material count:"))
				ThrowCurruptedFileException();
			infile >> m_header.materialCount;
			if (!ScanText(infile, L"Bounding volume primitive:"))
				ThrowCurruptedFileException();
			infile >> m_header.boundingVolumePrimitive;
			if (!ScanText(infile, L"Hitbox triangle count:"))
				ThrowCurruptedFileException();
			infile >> m_header.hitboxTriangleCount;
			if (!ScanText(infile, L"Bone count:"))
				ThrowCurruptedFileException();
			infile >> m_header.boneCount;
			if (!ScanText(infile, L"Animation count:"))
				ThrowCurruptedFileException();
			infile >> m_header.animationCount;

			m_data.boundingVolumeType = m_header.boundingVolumePrimitive;
			m_data.modelType = m_header.modelType;
			m_data.vertexCount = m_header.vertexCount;
			m_data.vertexSizeInBytes = gfx::ModelType::VertexSizeInBytes(m_header.modelType);
		}
		void OMDLoader::ReadVerticesText(std::wifstream& infile)
		{
			if (!ScanText(infile, L"Vertices:"))
				ThrowCurruptedFileException();
			m_data.vertices.resize(gfx::ModelType::VertexSizeInVertexElements(m_header.modelType) * m_header.vertexCount);
			if (!gfx::ModelType::HasBones(m_header.modelType))
			{
				for (gfx::VertexElement& v : m_data.vertices)
					infile >> v.f;
			}
			else
			{
				unsigned counter = 0;
				unsigned boneIndexOffset = gfx::ModelType::BoneIndexOffset(m_header.modelType);
				for (unsigned v = 0; v < m_header.vertexCount; v++)
				{
					for (unsigned f = 0; f < boneIndexOffset; f++)
						infile >> m_data.vertices[counter++].f;
					for (unsigned u = 0; u < 4; u++)
						infile >> m_data.vertices[counter++].u;
				}
			}
		}
		void OMDLoader::ReadIndicesText(std::wifstream& infile)
		{
			if (!ScanText(infile, L"Indices:"))
				ThrowCurruptedFileException();
			m_data.indices.resize(m_header.indexCount);
			for (unsigned& i : m_data.indices)
				infile >> i;
		}
		void OMDLoader::ReadGroupsText(std::wifstream& infile)
		{
			if (!ScanText(infile, L"Groups:"))
				ThrowCurruptedFileException();
			m_data.vertexGroups.resize(m_header.groupCount);
			for (auto& g : m_data.vertexGroups)
			{
				if (!ScanText(infile, L"New group"))
					ThrowCurruptedFileException();
				if (!ScanText(infile, L"Start index:"))
					ThrowCurruptedFileException();
				infile >> g.startIndex;
				if (!ScanText(infile, L"Index count:"))
					ThrowCurruptedFileException();
				infile >> g.indexCount;
				if (!ScanText(infile, L"Material index:"))
					ThrowCurruptedFileException();
				infile >> g.materialIndex;
			}
		}
		void OMDLoader::ReadMaterialsText(std::wifstream& infile)
		{
			if (!ScanText(infile, L"Materials:"))
				ThrowCurruptedFileException();
			m_data.materials.resize(m_header.materialCount);
			for (auto& m : m_data.materials)
			{
				if (!ScanText(infile, L"New material"))
					ThrowCurruptedFileException();
				if (!ScanText(infile, L"Texture name:"))
					ThrowCurruptedFileException();
				ScanLineEnd(infile, m.texture.filename);
				if (!m.texture.filename.empty())
					m.texture.filename = m_foldername + m.texture.filename;
				if (!ScanText(infile, L"Normalmap name:"))
					ThrowCurruptedFileException();
				ScanLineEnd(infile, m.normalmap.filename);
				if (!m.normalmap.filename.empty())
					m.normalmap.filename = m_foldername + m.normalmap.filename;
			}
		}
		void OMDLoader::ReadHitboxText(std::wifstream& infile)
		{
			if (ScanText(infile, L"Bounding volume type:"))
				ThrowCurruptedFileException();
			switch (m_header.boundingVolumePrimitive)
			{
			case physx::BoundingVolume::CUBOID:
				infile >> m_data.bvPosition >> m_data.bvCuboidSize;
				break;
			case physx::BoundingVolume::SPHERE:
				infile >> m_data.bvPosition >> m_data.bvSphereRadius;
				break;
			}
			if (!ScanText(infile, L"Hitbox:"))
				ThrowCurruptedFileException();
			m_data.hitboxVertices.resize(m_header.hitboxTriangleCount);
			for (auto& t : m_data.hitboxVertices)
			{
				mth::float3 triangle[3];
				mth::float3 plainNormal;
				float plainDistance;
				infile >> triangle[0] >> triangle[1] >> triangle[2] >> plainNormal >> plainDistance;
				//t = mth::Triangle3D(triangle, plainNormal, plainDistance);
			}
		}
		void OMDLoader::ReadBonesText(std::wifstream& infile)
		{
			if (!ScanText(infile, L"Bones:"))
				ThrowCurruptedFileException();
		}
		void OMDLoader::ReadAnimationsText(std::wifstream& infile)
		{
			if (!ScanText(infile, L"Animations:"))
				ThrowCurruptedFileException();
		}

#pragma endregion

#pragma region Load OMD binary

		void OMDLoader::LoadOMDBinary()
		{
		}

#pragma endregion

	}
}