#pragma once

namespace octdoc
{
	namespace gfx
	{
		union VertexElement
		{
			float f;
			unsigned u;

			inline float operator=(float n) { return f = n; }
			inline unsigned operator=(unsigned n) { return u = n; }
		};

		struct VertexGroup
		{
			unsigned startIndex;
			unsigned indexCount;
			int materialIndex;
		};

		namespace ModelType
		{
			enum Part :unsigned 
			{
				POSITION = 1 << 0,
				TEXCOORD = 1 << 1,
				NORMAL = 1 << 2,
				TANGENT = 1 << 3,
				BONE = 1 << 4,
				TEXTURE = 1 << 5,
				NORMALMAP = 1 << 6
			};

			constexpr unsigned P = POSITION;
			constexpr unsigned PT = POSITION | TEXCOORD | TEXTURE;
			constexpr unsigned PN = POSITION | NORMAL;
			constexpr unsigned PTN = POSITION | TEXCOORD | NORMAL | TEXTURE;
			constexpr unsigned PM = POSITION | TEXCOORD | NORMAL | TANGENT | NORMALMAP;
			constexpr unsigned PTM = POSITION | TEXCOORD | NORMAL | TANGENT | TEXTURE | NORMALMAP;
			constexpr unsigned PB = POSITION | BONE;
			constexpr unsigned PTB = POSITION | TEXCOORD | TEXTURE | BONE;
			constexpr unsigned PNB = POSITION | NORMAL | BONE;
			constexpr unsigned PTNB = POSITION | TEXCOORD | NORMAL | TEXTURE | BONE;
			constexpr unsigned PMB = POSITION | TEXCOORD | NORMAL | TANGENT | NORMALMAP | BONE;
			constexpr unsigned PTMB = POSITION | TEXCOORD | NORMAL | TANGENT | TEXTURE | NORMALMAP | BONE;
			constexpr unsigned AllPart = POSITION | TEXCOORD | NORMAL | TANGENT | TEXTURE | NORMALMAP | BONE;

			inline bool HasPositions(unsigned modelType) { return (bool)(modelType & Part::POSITION); }
			inline bool HasTexcoords(unsigned modelType) { return (bool)(modelType & Part::TEXCOORD); }
			inline bool HasNormals(unsigned modelType) { return (bool)(modelType & Part::NORMAL); }
			inline bool HasTangents(unsigned modelType) { return (bool)(modelType & Part::TANGENT); }
			inline bool HasBones(unsigned modelType) { return (bool)(modelType & Part::BONE); }
			inline bool HasTexture(unsigned modelType) { return (bool)(modelType & Part::TEXTURE); }
			inline bool HasNormalmap(unsigned modelType) { return (bool)(modelType & Part::NORMALMAP); }
			unsigned VertexSizeInBytes(unsigned modelType);
			unsigned VertexSizeInVertexElements(unsigned modelType);
			unsigned PositionOffset(unsigned modelType);
			unsigned TexCoordOffset(unsigned modelType);
			unsigned NormalOffset(unsigned modelType);
			unsigned TangentOffset(unsigned modelType);
			unsigned BoneWeightsOffset(unsigned modelType);
			unsigned BoneIndexOffset(unsigned modelType);
			unsigned RemoveUnnecessary(unsigned modelType);
			unsigned ToModelType(bool position, bool texcoord, bool normal, bool tangent, bool bone, bool texture, bool normalmap);
			inline unsigned VertexLayout(unsigned modelType) { return modelType & (Part::POSITION | Part::TEXCOORD | Part::NORMAL | Part::TANGENT | Part::BONE); }
		};
	}
}