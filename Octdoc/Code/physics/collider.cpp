#include "collider.h"

namespace octdoc
{
	namespace physx
	{
		CollisionData::CollisionData() :time(1.0f), normal() {}
		void Collider::CalculateAABB()
		{
			if (m_hitboxMesh.empty())
			{
				mth::float3 min = m_hitboxMesh[0].getVertex(0);
				mth::float3 max = min;
				for (mth::Triangle3D& tri : m_hitboxMesh)
				{
					for (int i = 0; i < 3; i++)
					{
						mth::float3 vertex = tri.getVertex(i);
						if (vertex.x < min.x) min.x = vertex.x;
						if (vertex.y < min.y) min.y = vertex.y;
						if (vertex.z < min.z) min.z = vertex.z;
						if (vertex.x > max.x) max.x = vertex.x;
						if (vertex.y > max.y) max.y = vertex.y;
						if (vertex.z > max.z) max.z = vertex.z;
					}
				}
				m_aabb.position = min;
				m_aabb.size = max - min;
			}
		}
		bool Collider::BoundingVolumeIntersects(mth::Position& ellipsoid)
		{
			//translate and scale bounding volume
			//don't rotate, so it stays aabb
			BV_Cuboid bvCollider(position + scale * m_aabb.position, scale * m_aabb.size);
			//ellipsoid gets a bounding sphere
			//ellipsoid's spherical bounding volume gets rotated in opposite direction
			BV_Sphere bvEllipsoid(position * mth::float3x3::Rotation(rotation), ellipsoid.scale.Max());
			return bvCollider.Intersects(bvEllipsoid);
		}
		Collider::Collider(hlp::ModelLoader& loader)
		{
			if (!loader.HasHitbox())
				throw std::exception("Model has no hitbox");
			//m_hitboxMesh = loader.getHitboxMesh();
			switch (loader.getBoundingVolumePrimitive())
			{
			case BoundingVolume::CUBOID:
				m_aabb.position = loader.getBoundingVolumePosition();
				m_aabb.size = loader.getBoundingVolumeCubiodSize();
				break;
			case BoundingVolume::SPHERE:
				m_aabb.position = loader.getBoundingVolumePosition() - loader.getBoundingVolumeSphereRadius();
				m_aabb.size = 2.0f * loader.getBoundingVolumeSphereRadius();
				break;
			case BoundingVolume::NO_TYPE:
				CalculateAABB();
				break;
			}
		}

		Collider::Collider(mth::float3 triangles[], int triangleCount)
		{
			m_hitboxMesh.reserve(triangleCount);
			for (int i = 0; i < triangleCount; i++)
			{
				m_hitboxMesh.push_back(mth::Triangle3D(&triangles[3 * i]));
			}
		}
		Collider::U Collider::CreateU(hlp::ModelLoader& loader)
		{
			return std::make_unique<Collider>(loader);
		}
		Collider::U Collider::CreateU(mth::float3 triangles[], int triangleCount)
		{
			return std::make_unique<Collider>(triangles, triangleCount);
		}
		Collider::P Collider::CreateP(hlp::ModelLoader& loader)
		{
			return std::make_shared<Collider>(loader);
		}
		Collider::P Collider::CreateP(mth::float3 triangles[], int triangleCount)
		{
			return std::make_shared<Collider>(triangles, triangleCount);
		}
		bool Collider::CollidesWithEllipsoid(mth::Position ellipsoid, mth::float3 velocity, CollisionData& collisionData)
		{
			if (!BoundingVolumeIntersects(ellipsoid))
				return false;
			mth::float4x4 transformMatrix = GetWorldMatrix() * ellipsoid.GetWorldMatrixInv();
			mth::float3 trPos = mth::Transform(transformMatrix, ellipsoid.position);	//transformed position
			mth::float3 trVel = mth::float3x3(transformMatrix) * velocity;	//transformed velocity, don't apply translation
			float previousCollTime = collisionData.time;	//save previous collision so we know if collision happenes here
			for (mth::Triangle3D& t : m_hitboxMesh)
			{
				mth::Triangle3D triangle(
					mth::Transform(transformMatrix, t.getVertex(0)),
					mth::Transform(transformMatrix, t.getVertex(1)),
					mth::Transform(transformMatrix, t.getVertex(2)));
				//how long till it reaches the triangle plain
				float time = triangle.getPlain().TimeToGetClose(trPos, trVel, 1.0f);
				if (time < 0.0f)	//goes opposite direction
					continue;
				if (time < collisionData.time)	//reaches current plain before any other
				{
					if (triangle.IsPointOver(trPos + time * velocity))	//hits the triangle face
					{
						collisionData.time = time;
						collisionData.normal = triangle.getPlainNormal();
						continue;
					}
					//misses the face, check if vertices got hit
					for (int i = 0; i < 3; i++)
					{
						mth::Point3D point(triangle.getVertex(i));
						time = point.TimeToGetClose(trPos, trVel, 1.0f);
						if (time > 0.0f && time < collisionData.time)
						{
							collisionData.time = time;
							collisionData.normal = point.p - trPos;
							continue;
						}
					}
					//misses vertices too, check edges
					for (int i = 0; i < 3; i++)
					{
						mth::float3 p1 = triangle.getVertex(i), p2 = triangle.getVertex((i + 1) % 3);
						mth::Line3D line(p1, p2 - p1);
						time = line.TimeToGetClose(trPos, trVel, 1.0f);
						if (time > 0.0f && time < collisionData.time)
						{
							//check if line is hit between vertices
							float len = line.v.Dot(trPos + trVel * time - p1);
							if (len > 0.0f && len < 1.0f)
							{
								collisionData.time = time;
								collisionData.normal = line.p + line.v * time - trPos;
								continue;
							}
						}
					}
				}
			}
			//no collision happened
			if (collisionData.time == previousCollTime)
				return false;
			//collision happened
			//transform normal back to original space
			transformMatrix.Transpose();
			collisionData.normal = mth::Transform(transformMatrix, collisionData.normal);
			return true;
		}
	}
}