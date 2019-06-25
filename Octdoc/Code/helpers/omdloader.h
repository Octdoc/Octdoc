#pragma once

#include "modelloader.h"

namespace octdoc
{
	namespace hlp
	{
		class OMDLoader
		{
			struct OMDHeader
			{
				char fileFormat;	//'t' for text, 'b' for binary
				char extension[3];	//"OMD"
				unsigned modelType;
				unsigned vertexCount;
				unsigned indexCount;
				unsigned groupCount;
				unsigned materialCount;
				unsigned boundingVolumePrimitive;
				unsigned hitboxTriangleCount;
				unsigned boneCount;
				unsigned animationCount;

				OMDHeader();
			};

			ModelData& m_data;
			OMDHeader m_header;
			std::wstring m_filename;
			std::wstring m_foldername;

		private:
			void ThrowCurruptedFileException();

			void LoadOMDText();
			void ReadHeaderText(std::wifstream& infile);
			void ReadVerticesText(std::wifstream& infile);
			void ReadIndicesText(std::wifstream& infile);
			void ReadGroupsText(std::wifstream& infile);
			void ReadMaterialsText(std::wifstream& infile);
			void ReadHitboxText(std::wifstream& infile);
			void ReadBonesText(std::wifstream& infile);
			void ReadAnimationsText(std::wifstream& infile);

			void LoadOMDBinary();

		public:
			OMDLoader(ModelData& data);
			void LoadOMD(const wchar_t* filename);
		};
	}
}