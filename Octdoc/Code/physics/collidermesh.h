#pragma once

#include "collider.h"
#include <vector>

namespace octdoc
{
	namespace physx
	{
		class Collider_Mesh :public Collider
		{
			SMART_PTR(Collider_Mesh)
		private:
			BV_Cuboid m_aabb;
			std::vector<mth::double3> m_hitboxVertices;
			std::vector<unsigned> m_hitboxIndices;
			
		public:
			Collider_Mesh(hlp::ModelLoader& loader);
			static Collider_Mesh::U CreateU(hlp::ModelLoader& loader);
			static Collider_Mesh::P CreateP(hlp::ModelLoader& loader);

			inline BV_Cuboid& getBoundingVolume() { return m_aabb; }
			inline unsigned getTriangleCount() { return m_hitboxIndices.size() / 3; }
			inline mth::double3 getTriangleVertex(unsigned triIndex, unsigned vertIndex) { return m_hitboxVertices[m_hitboxIndices[3 * triIndex + vertIndex]]; }

			virtual bool Collides(Collider& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData) override;
			virtual bool Collides(Collider_Ellipsoid& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData) override;
			virtual bool Collides(Collider_Mesh& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData) override;
		};
	}
}