#pragma once

#include "colliderellipsoid.h"
#include "collidermesh.h"

namespace octdoc
{
	namespace physx
	{
		class Collides_EllipsoidMesh
		{
			Collider_Ellipsoid& m_ellipsoid;
			mth::double3 m_vEllipsoid;
			Collider_Mesh& m_mesh;
			mth::double3 m_vMesh;
			CollisionData& m_collData;

		private:
			bool BoundingVolumeIntersects();
			bool CollideWithFace(mth::Triangle3Dd& triangle, mth::double3 v);
			bool CollideWithEdges(mth::Triangle3Dd& triangle, mth::double3 v);
			bool CollideWithVertices(mth::Triangle3Dd& triangle, mth::double3 v);

		public:
			Collides_EllipsoidMesh(Collider_Ellipsoid& c1, mth::double3 v1, Collider_Mesh& c2, mth::double3 v2, CollisionData& collisionData);
			bool Collides();
		};
	}
}