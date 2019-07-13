#include "collidermesh.h"

namespace octdoc
{
	namespace physx
	{
		Collider_Mesh::Collider_Mesh(hlp::ModelLoader& loader)
		{
			m_hitboxVertices = loader.getHitboxVertices();
			m_hitboxIndices = loader.getHitboxIndices();
			m_aabb.position = loader.getBoundingVolumePosition();
			m_aabb.size= loader.getBoundingVolumeCubiodSize();
		}
		Collider_Mesh::U Collider_Mesh::CreateU(hlp::ModelLoader& loader)
		{
			return std::make_unique<Collider_Mesh>(loader);
		}
		Collider_Mesh::P Collider_Mesh::CreateP(hlp::ModelLoader& loader)
		{
			return std::make_shared<Collider_Mesh>(loader);
		}
		bool Collider_Mesh::Collides(Collider& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData)
		{
			if (collider2.Collides(*this, v2, v1, collisionData))
			{
				collisionData.normal *= -1;
				return true;
			}
			return false;
		}
		bool Collider_Mesh::Collides(Collider_Ellipsoid& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData)
		{
			if (Collides_EM(collider2, v2, *this, v1, collisionData))
			{
				collisionData.normal *= -1;
				return true;
			}
			return false;
		}
		bool Collider_Mesh::Collides(Collider_Mesh& collider2, mth::double3 v1, mth::double3 v2, CollisionData& collisionData)
		{
			return Collides_MM(*this, v1, collider2, v2, collisionData);
		}
	}
}