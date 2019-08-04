#pragma once

#include "graphics.h"
#include "helpers/modelloader.h"

namespace octdoc
{
	namespace gfx
	{
		class Model
		{
			SMART_PTR(Model)
		protected:
			unsigned m_vertexSizeInBytes;
			unsigned m_modelType;
			unsigned m_vertexCount;
			unsigned m_indexCount;

			struct Group
			{
				unsigned startIndex;
				unsigned indexCount;
			};
			std::vector<Group> m_groups;

		public:
			static Model::P CreateP(Graphics& graphics, hlp::ModelLoader& loader);
			static Model::U CreateU(Graphics& graphics, hlp::ModelLoader& loader);

			inline unsigned getGroupCount() { return (unsigned)m_groups.size(); }
			inline unsigned getModelType() { return m_modelType; }
			virtual void SetBuffersToRender(Graphics& graphics) = 0;
			virtual void RenderGroup(Graphics& graphics, unsigned index) = 0;
			virtual void RenderAll(Graphics& graphics) = 0;
		};
	}
}