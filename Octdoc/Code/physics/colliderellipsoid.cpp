#include "colliderellipsoid.h"

namespace octdoc
{
	namespace physx
	{
		Collider_Ellipsoid::Collider_Ellipsoid(mth::double3 ellRadii) :m_ellRadii(ellRadii) {}
		Collider_Ellipsoid::Collider_Ellipsoid(hlp::ModelLoader& loader)
		{
			throw std::exception("Cannot create ellipsoid collider");
		}
		Collider_Ellipsoid::U Collider_Ellipsoid::CreateU(mth::double3 ellRadii)
		{
			return std::make_unique<Collider_Ellipsoid>(ellRadii);
		}
		Collider_Ellipsoid::U Collider_Ellipsoid::CreateU(hlp::ModelLoader& loader)
		{
			return std::make_unique<Collider_Ellipsoid>(loader);
		}
		Collider_Ellipsoid::P Collider_Ellipsoid::CreateP(mth::double3 ellRadii)
		{
			return std::make_shared<Collider_Ellipsoid>(ellRadii);
		}
		Collider_Ellipsoid::P Collider_Ellipsoid::CreateP(hlp::ModelLoader& loader)
		{
			return std::make_shared<Collider_Ellipsoid>(loader);
		}
		bool Collider_Ellipsoid::Collides(Collider& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData)
		{
			if (collider2.Collides(*this, v2, v1, collisionData))
			{
				collisionData.normal *= -1;
				return true;
			}
			return false;
		}
		bool Collider_Ellipsoid::Collides(Collider_Ellipsoid& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData)
		{
			return Collides_EE(*this, v1, collider2, v2, collisionData);
		}
		bool Collider_Ellipsoid::Collides(Collider_Mesh& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData)
		{
			return Collides_EM(*this, v1, collider2, v2, collisionData);
		}
	}
}