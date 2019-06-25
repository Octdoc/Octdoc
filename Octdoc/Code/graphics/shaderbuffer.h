#pragma once

#include "graphics.h"

namespace octdoc
{
	namespace gfx
	{
		class ShaderBuffer
		{
			SMART_PTR(ShaderBuffer)
		protected:
			unsigned m_bufferSize;

		public:
			static ShaderBuffer::P CreateP(Graphics& graphics, unsigned size);
			static ShaderBuffer::U CreateU(Graphics& graphics, unsigned size);

			virtual bool WriteBuffer(Graphics& graphics, void *data) = 0;
			virtual void SetToVertexShader(Graphics& graphics, unsigned index = 0) = 0;
			virtual void SetToPixelShader(Graphics& graphics, unsigned index = 0) = 0;
		};
	}
}