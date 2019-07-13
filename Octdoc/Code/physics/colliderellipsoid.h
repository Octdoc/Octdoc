#pragma once

#include "collider.h"

namespace octdoc
{
	namespace physx
	{
		class Collider_Ellipsoid :public Collider
		{
			SMART_PTR(Collider_Ellipsoid)
		private:
			mth::double3 m_ellRadii;

		public:
			Collider_Ellipsoid(mth::double3 ellRadii);
			Collider_Ellipsoid(hlp::ModelLoader& loader);
			static Collider_Ellipsoid::U CreateU(mth::double3 ellRadii);
			static Collider_Ellipsoid::U CreateU(hlp::ModelLoader& loader);
			static Collider_Ellipsoid::P CreateP(mth::double3 ellRadii);
			static Collider_Ellipsoid::P CreateP(hlp::ModelLoader& loader);

			inline mth::double3 getRadii() { return m_ellRadii; }

			virtual bool Collides(Collider& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData) override;
			virtual bool Collides(Collider_Ellipsoid& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData) override;
			virtual bool Collides(Collider_Mesh& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData) override;
		};
	}
}