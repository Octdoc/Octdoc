#pragma once

#include "helpers/helpers.h"
#include "helpers/modelloader.h"
#include "boundingvolume.h"
#include "geometry3d.hpp"
#include <vector>

namespace octdoc
{
	namespace physx
	{
		struct CollisionData
		{
			//negative time if not colliding
			double time;
			mth::double3 normal;
			mth::double3 point;

			CollisionData();
		};

		class Collider :public mth::Position<double>
		{
			SMART_PTR(Collider)
		private:
			BV_Cuboid m_aabb;
			std::vector<mth::double3> m_hitboxVertices;
			std::vector<unsigned> m_hitboxIndices;

		private:
			void CalculateAABB();
			bool BoundingVolumeIntersects(mth::Position<double> ellipsoid, mth::double3 velocity);

		public:
			Collider(hlp::ModelLoader& loader);
			Collider(mth::double3 triangles[], int triangleCount);
			static Collider::U CreateU(hlp::ModelLoader& loader);
			static Collider::U CreateU(mth::double3 triangles[], int triangleCount);
			static Collider::P CreateP(hlp::ModelLoader& loader);
			static Collider::P CreateP(mth::double3 triangles[], int triangleCount);

			inline BV_Cuboid& getBoundingVolume() { return m_aabb; }

			bool CollidesWithEllipsoid(mth::Position<double> ellipsoid, mth::double3 velocity, CollisionData& collisionData);
		};
	}
}