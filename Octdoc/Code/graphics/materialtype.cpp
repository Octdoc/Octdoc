#include "materialtype.h"


namespace octdoc
{
	namespace gfx
	{
		namespace MaterialType
		{
			const wchar_t* VertexShaderName(unsigned materialType)
			{
				return L"Shaders/vertexShader.cso";
			}
			const wchar_t* PixelShaderName(unsigned materialType)
			{
				return L"Shaders/pixelShader.cso";
			}
		}
	}
}