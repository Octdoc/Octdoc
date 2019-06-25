#pragma once

#include "helpers.h"
#include <vector>

namespace octdoc
{
	namespace hlp
	{
		void LoadTargaFromFile(const wchar_t* filename, std::vector<uint8>& pixelData, unsigned& width, unsigned& height);
	}
}