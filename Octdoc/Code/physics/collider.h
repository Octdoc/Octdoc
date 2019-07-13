#pragma once

#include "helpers/helpers.h"
#include "helpers/modelloader.h"
#include "boundingvolume.h"
#include "geometry3d.hpp"

namespace octdoc
{
	namespace physx
	{
		struct CollisionData
		{
			double time;
			mth::double3 normal;
			mth::double3 point;

			CollisionData();
		};

		class Collider_Ellipsoid;
		class Collider_Mesh;

		class Collider :public mth::Position<double>
		{
			SMART_PTR(Collider)
		public:
			static Collider::U CreateU(hlp::ModelLoader& loader);
			static Collider::P CreateP(hlp::ModelLoader& loader);

			virtual bool Collides(Collider& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData) = 0;
			virtual bool Collides(Collider_Ellipsoid& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData) = 0;
			virtual bool Collides(Collider_Mesh& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData) = 0;
		};

		bool Collides_CC(Collider& c1, mth::double3 v1, Collider& c2, mth::double3 v2, CollisionData& collisionData);
		bool Collides_EE(Collider_Ellipsoid& c1, mth::double3 v1, Collider_Ellipsoid& c2, mth::double3 v2, CollisionData& collisionData);
		bool Collides_EM(Collider_Ellipsoid& c1, mth::double3 v1, Collider_Mesh& c2, mth::double3 v2, CollisionData& collisionData);
		bool Collides_MM(Collider_Mesh& c1, mth::double3 v1, Collider_Mesh& c2, mth::double3 v2, CollisionData& collisionData);
	}
}