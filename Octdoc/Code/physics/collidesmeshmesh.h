#pragma once

#include "colliderellipsoid.h"
#include "collidermesh.h"

namespace octdoc
{
	namespace physx
	{
		class Collides_MeshMesh
		{
			Collider_Mesh& m_mesh1;
			mth::double3 m_v1;
			Collider_Mesh& m_mesh2;
			mth::double3 m_v2;
			CollisionData& m_collData;

		private:
			bool BoundingVolumeIntersects();
			bool VerticesHitFace(mth::Triangle3Dd& tri1, mth::Triangle3Dd& tri2, mth::double3 v);
			bool EdgesHitEdges(mth::Triangle3Dd& tri1, mth::Triangle3Dd& tri2, mth::double3 v);

		public:
			Collides_MeshMesh(Collider_Mesh& c1, mth::double3 v1, Collider_Mesh& c2, mth::double3 v2, CollisionData& collisionData);
			bool Collides();
		};
	}
}