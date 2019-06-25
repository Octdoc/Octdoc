#pragma once

#include "helpers/helpers.h"
#include "helpers/modelloader.h"
#include "boundingvolume.h"
#include "position.h"
#include "geometry3d.h"
#include <vector>

namespace octdoc
{
	namespace physx
	{
		struct CollisionData
		{
			//negative time if not colliding
			float time;
			mth::float3 normal;

			CollisionData();
		};

		class Collider :public mth::Position
		{
			SMART_PTR(Collider)
		private:
			BV_Cuboid m_aabb;
			std::vector<mth::Triangle3D> m_hitboxMesh;

		private:
			void CalculateAABB();
			bool BoundingVolumeIntersects(mth::Position& ellipsoid);

		public:
			Collider(hlp::ModelLoader& loader);
			Collider(mth::float3 triangles[], int triangleCount);
			static Collider::U CreateU(hlp::ModelLoader& loader);
			static Collider::U CreateU(mth::float3 triangles[], int triangleCount);
			static Collider::P CreateP(hlp::ModelLoader& loader);
			static Collider::P CreateP(mth::float3 triangles[], int triangleCount);

			inline BV_Cuboid& getBoundingVolume() { return m_aabb; }

			bool CollidesWithEllipsoid(mth::Position ellipsoid, mth::float3 velocity, CollisionData& collisionData);
		};
	}
}