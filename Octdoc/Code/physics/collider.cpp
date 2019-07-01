#include "collider.h"

namespace octdoc
{
	namespace physx
	{
		CollisionData::CollisionData() :time(1.0f), normal(), point() {}

		void Collider::CalculateAABB()
		{
			if (m_hitboxVertices.empty())
			{
				mth::double3 min = m_hitboxVertices[0];
				mth::double3 max = min;
				for (mth::double3& v : m_hitboxVertices)
				{
					if (v.x < min.x) min.x = v.x;
					if (v.y < min.y) min.y = v.y;
					if (v.z < min.z) min.z = v.z;
					if (v.x > max.x) max.x = v.x;
					if (v.y > max.y) max.y = v.y;
					if (v.z > max.z) max.z = v.z;
				}
				m_aabb.position = min;
				m_aabb.size = max - min;
			}
		}
		bool Collider::BoundingVolumeIntersects(mth::Position<double> ellipsoid, mth::double3 velocity)
		{
			//ellipsoid gets a bounding sphere with transformed centre
			BV_Sphere bvEllipsoid(((ellipsoid.position + velocity) * mth::double3x3::Rotation(rotation)) / scale, ellipsoid.scale.Max());
			return m_aabb.Intersects(bvEllipsoid);
		}
		Collider::Collider(hlp::ModelLoader& loader)
		{
			if (!loader.HasHitbox())
				throw std::exception("Model has no hitbox");
			m_hitboxVertices = loader.getHitboxVertices();
			m_hitboxIndices = loader.getHitboxIndices();
			switch (loader.getBoundingVolumePrimitive())
			{
			case BoundingVolume::CUBOID:
				m_aabb.position = loader.getBoundingVolumePosition();
				m_aabb.size = loader.getBoundingVolumeCubiodSize();
				break;
			case BoundingVolume::SPHERE:
				m_aabb.position = loader.getBoundingVolumePosition() - loader.getBoundingVolumeSphereRadius();
				m_aabb.size = 2 * loader.getBoundingVolumeSphereRadius();
				break;
			case BoundingVolume::NO_TYPE:
				CalculateAABB();
				break;
			}
		}

		Collider::Collider(mth::double3 triangles[], int triangleCount)
		{
			m_hitboxVertices.reserve(triangleCount);
			m_hitboxIndices.reserve(triangleCount);
			for (int i = 0; i < triangleCount * 3; i++)
			{
				m_hitboxVertices.push_back(triangles[i]);
				m_hitboxIndices.push_back(i);
			}
		}
		Collider::U Collider::CreateU(hlp::ModelLoader& loader)
		{
			return std::make_unique<Collider>(loader);
		}
		Collider::U Collider::CreateU(mth::double3 triangles[], int triangleCount)
		{
			return std::make_unique<Collider>(triangles, triangleCount);
		}
		Collider::P Collider::CreateP(hlp::ModelLoader& loader)
		{
			return std::make_shared<Collider>(loader);
		}
		Collider::P Collider::CreateP(mth::double3 triangles[], int triangleCount)
		{
			return std::make_shared<Collider>(triangles, triangleCount);
		}
		bool Collider::CollidesWithEllipsoid(mth::Position<double> ellipsoid, mth::double3 velocity, CollisionData& collisionData)
		{
			if (!BoundingVolumeIntersects(ellipsoid, velocity))
				return false;
			mth::double4x4 ellipsoidWorldInv = ellipsoid.GetWorldMatrixInv();
			mth::double3x3 ellipsoidNoTranslateInv(ellipsoidWorldInv);
			mth::double4x4 transformMatrix = ellipsoidWorldInv * (mth::double4x4)GetWorldMatrix();
			mth::double3 trVel = ellipsoidNoTranslateInv * velocity;	//transformed velocity
			double previousCollTime = collisionData.time;	//save previous collision so we know if collision happenes here
			for (unsigned i = 0; i < m_hitboxIndices.size(); i += 3)
			{
				mth::Triangle3Dd triangle(
					mth::Transform(transformMatrix, m_hitboxVertices[m_hitboxIndices[i + 0]]),
					mth::Transform(transformMatrix, m_hitboxVertices[m_hitboxIndices[i + 1]]),
					mth::Transform(transformMatrix, m_hitboxVertices[m_hitboxIndices[i + 2]]));
				//if won't get close to the plain to intersect, skip
				if (triangle.getPlain().Distance(trVel) > 1.0)
					continue;
				//how long till it reaches the triangle plain
				double time = triangle.getPlain().TimeToGetClose(mth::double3(), trVel, 1.0);
				if (time < collisionData.time)	//reaches current plain before any other
				{
					if (time >= 0.0)	//over the plain
					{
						if (triangle.IsPointOver(time * trVel))	//hits the triangle face
						{
							collisionData.time = time;
							collisionData.normal = triangle.getPlainNormal();
							continue;
						}
					}
				}
				//misses the face, check if vertices got hit
				for (unsigned v = 0; v < 3; v++)
				{
					mth::Point3Dd point(triangle.getVertex(v));
					time = point.TimeToGetClose(mth::double3(), trVel, 1.0);
					if (time > 0.0 && time < collisionData.time)
					{
						collisionData.time = time;
						collisionData.normal = point.getPoint();
					}
				}
				//check edges
				for (unsigned v = 0; v < 3; v++)
				{
					mth::double3 p1 = triangle.getVertex(v), p2 = triangle.getVertex((v + 1) % 3);
					double lineLen = (p2 - p1).Length();
					mth::Line3Dd line(p1, (p2 - p1) / lineLen);
					time = line.TimeToGetClose(mth::double3(), trVel, 1.0);
					if (time > 0.0 && time < collisionData.time)
					{
						//check if line is hit between vertices
						double len = line.getDirection().Dot(trVel * time - line.getPoint());
						if (len >= 0.0 && len <= lineLen)
						{
							collisionData.time = time;
							collisionData.normal = line.getPoint() + line.getDirection() * len;
						}
					}
				}
			}
			//no collision happened
			if (collisionData.time == previousCollTime)
				return false;
			//collision happened
			//transform normal back to original space
			collisionData.point = mth::Transform(ellipsoid.GetWorldMatrix(), collisionData.normal);
			collisionData.normal = (collisionData.normal * ellipsoidNoTranslateInv).Normalized();
			return true;
		}
	}
}