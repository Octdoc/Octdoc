#pragma once

namespace octdoc
{
	namespace gfx
	{
		namespace MaterialType
		{
			enum Part :unsigned
			{
				TEXTURE_POINTLIGHT = 1
			};
			const wchar_t* VertexShaderName(unsigned materialType);
			const wchar_t* PixelShaderName(unsigned materialType);
		};
	}
}