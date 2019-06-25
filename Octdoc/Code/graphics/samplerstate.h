#pragma once

#include "graphics.h"

namespace octdoc
{
	namespace gfx
	{
		class SamplerState
		{
			SMART_PTR(SamplerState)
		public:
			static SamplerState::P CreateP(Graphics& graphics, bool interpolate, bool repeatedWrap);
			static SamplerState::U CreateU(Graphics& graphics, bool interpolate, bool repeatedWrap);

			virtual void SetToPixelShader(Graphics& graphics, unsigned index = 0) = 0;
		};
	}
}