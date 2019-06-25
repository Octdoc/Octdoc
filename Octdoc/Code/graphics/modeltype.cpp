#include "modeltype.h"

namespace octdoc
{
	namespace gfx
	{
		namespace ModelType
		{
			unsigned VertexSizeInBytes(unsigned modelType)
			{
				return VertexSizeInVertexElements(modelType) * sizeof(VertexElement);
			}
			unsigned VertexSizeInVertexElements(unsigned modelType)
			{
				unsigned size = 0;
				if (HasPositions(modelType))
					size += 3;
				if (HasTexcoords(modelType))
					size += 2;
				if (HasNormals(modelType))
					size += 3;
				if (HasTangents(modelType))
					size += 3;
				if (HasBones(modelType))
					size += 8;
				return size;
			}
			unsigned PositionOffset(unsigned modelType)
			{
				unsigned offset = 0;
				return offset;
			}
			unsigned TexCoordOffset(unsigned modelType)
			{
				unsigned offset = 0;
				if (HasPositions(modelType))
					offset += 3;
				return offset;
			}
			unsigned NormalOffset(unsigned modelType)
			{
				unsigned offset = 0;
				if (HasPositions(modelType))
					offset += 3;
				if (HasTexcoords(modelType))
					offset += 2;
				return offset;
			}
			unsigned TangentOffset(unsigned modelType)
			{
				unsigned offset = 0;
				if (HasPositions(modelType))
					offset += 3;
				if (HasTexcoords(modelType))
					offset += 2;
				if (HasNormals(modelType))
					offset += 3;
				return offset;
			}
			unsigned BoneWeightsOffset(unsigned modelType)
			{
				unsigned offset = 0;
				if (HasPositions(modelType))
					offset += 3;
				if (HasTexcoords(modelType))
					offset += 2;
				if (HasNormals(modelType))
					offset += 3;
				if (HasTangents(modelType))
					offset += 3;
				return offset;
			}
			unsigned BoneIndexOffset(unsigned modelType)
			{
				unsigned offset = 0;
				if (HasPositions(modelType))
					offset += 3;
				if (HasTexcoords(modelType))
					offset += 2;
				if (HasNormals(modelType))
					offset += 3;
				if (HasTangents(modelType))
					offset += 3;
				if (HasBones(modelType))
					offset += 4;
				return offset;
			}
			unsigned RemoveUnnecessary(unsigned modelType)
			{
				if (!(modelType & Part::POSITION))
					return 0;
				if (!(modelType & Part::TEXCOORD))
					modelType &= ~(Part::TEXTURE | Part::NORMALMAP);
				if (!(modelType & Part::NORMAL))
					modelType &= ~(Part::TANGENT | Part::NORMALMAP);
				if (!(modelType & Part::TANGENT))
					modelType &= ~Part::NORMALMAP;
				if (!(modelType & Part::NORMALMAP))
				{
					modelType &= ~Part::TANGENT;
					if (!Part::TEXTURE)
						modelType &= ~Part::TEXCOORD;
				}
				return modelType;
			}
			unsigned ToModelType(bool position, bool texcoord, bool normal, bool tangent, bool bone, bool texture, bool normalmap)
			{
				unsigned modelType = 0;
				if (position)
					modelType |= Part::POSITION;
				if (texcoord)
					modelType |= Part::TEXCOORD;
				if (normal)
					modelType |= Part::NORMAL;
				if (tangent)
					modelType |= Part::TANGENT;
				if (bone)
					modelType |= Part::BONE;
				if (texture)
					modelType |= Part::TEXTURE;
				if (normalmap)
					modelType |= Part::NORMALMAP;
				return modelType;
			}
		}
	}
}
