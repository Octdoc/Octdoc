#include "collidesmeshellipsoid.h"
#include "collidesmeshmesh.h"

namespace octdoc
{
	namespace physx
	{
		CollisionData::CollisionData() :time(1.0f), normal(), point() {}

		Collider::U Collider::CreateU(hlp::ModelLoader& loader)
		{
			return std::make_unique<Collider_Mesh>(loader);
		}
		Collider::P Collider::CreateP(hlp::ModelLoader& loader)
		{
			return std::make_shared<Collider_Mesh>(loader);
		}
		bool Collider::Collides(Collider& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData)
		{
			return collider2.Collides(*this, v2, v1, collisionData);
		}
		bool Collides_CC(Collider& c1, mth::double3 v1, Collider& c2, mth::double3 v2, CollisionData& collisionData)
		{
			return c1.Collides(c2, v1, v2, collisionData);
		}
		bool Collides_EE(Collider_Ellipsoid& c1, mth::double3 v1, Collider_Ellipsoid& c2, mth::double3 v2, CollisionData& collisionData)
		{
			return false;
		}
		bool Collides_EM(Collider_Ellipsoid& c1, mth::double3 v1, Collider_Mesh& c2, mth::double3 v2, CollisionData& collisionData)
		{
			return Collides_EllipsoidMesh(c1, v1, c2, v2, collisionData).Collides();
		}
		bool Collides_MM(Collider_Mesh& c1, mth::double3 v1, Collider_Mesh& c2, mth::double3 v2, CollisionData& collisionData)
		{
			return Collides_MeshMesh(c1, v1, c2, v2, collisionData).Collides();
		}
	}
}