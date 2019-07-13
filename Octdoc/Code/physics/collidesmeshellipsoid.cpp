#include "collidesmeshellipsoid.h"

namespace octdoc
{
	namespace physx
	{
		bool Collides_EllipsoidMesh::BoundingVolumeIntersects()
		{
			//ellipsoid gets a bounding sphere with transformed centre
			BV_Sphere bvEllipsoid(
				((m_ellipsoid.position + m_vEllipsoid) * mth::double3x3::Rotation(m_mesh.rotation)) /
				m_mesh.scale, (m_ellipsoid.scale * m_ellipsoid.getRadii()).Max());
			return m_mesh.getBoundingVolume().Intersects(bvEllipsoid);
		}
		bool Collides_EllipsoidMesh::CollideWithFace(mth::Triangle3Dd& triangle, mth::double3 v)
		{
			//how long till it reaches the triangle plain
			double time = triangle.getPlain().TimeToGetClose(mth::double3(), v, 1.0);
			if (time < m_collData.time)	//reaches current plain before any other
			{
				if (time >= 0.0)	//over the plain
				{
					if (triangle.IsPointOver(time * v))	//hits the triangle face
					{
						m_collData.time = time;
						m_collData.normal = triangle.getPlainNormal();
						return true;
					}
				}
			}
			return false;
		}
		bool Collides_EllipsoidMesh::CollideWithEdges(mth::Triangle3Dd& triangle, mth::double3 v)
		{
			bool coll = false;
			//check edges
			for (unsigned i = 0; i < 3; i++)
			{
				mth::double3 p1 = triangle.getVertex(i), p2 = triangle.getVertex((i + 1) % 3);
				double lineLen = (p2 - p1).Length();
				mth::Line3Dd line(p1, (p2 - p1) / lineLen);
				double time = line.TimeToGetClose(mth::double3(), v, 1.0);
				if (time > 0.0 && time < m_collData.time)
				{
					//check if line is hit between vertices
					double len = line.getDirection().Dot(i * time - line.getPoint());
					if (len >= 0.0 && len <= lineLen)
					{
						m_collData.time = time;
						m_collData.normal = line.getPoint() + line.getDirection() * len;
						coll = true;
					}
				}
			}
			return coll;
		}
		bool Collides_EllipsoidMesh::CollideWithVertices(mth::Triangle3Dd& triangle, mth::double3 v)
		{
			bool coll = false;
			//misses the face, check if vertices got hit
			for (unsigned i = 0; i < 3; i++)
			{
				mth::Point3Dd point(triangle.getVertex(i));
				double time = point.TimeToGetClose(mth::double3(), v, 1.0);
				if (time > 0.0 && time < m_collData.time)
				{
					m_collData.time = time;
					m_collData.normal = point.getPoint();
					coll = true;
				}
			}
			return coll;
		}
		Collides_EllipsoidMesh::Collides_EllipsoidMesh(Collider_Ellipsoid& c1, mth::double3 v1, Collider_Mesh& c2, mth::double3 v2, CollisionData& collisionData) :
			m_ellipsoid(c1),
			m_vEllipsoid(v1),
			m_mesh(c2),
			m_vMesh(v2),
			m_collData(collisionData) {}

		bool Collides_EllipsoidMesh::Collides()
		{
			if (!BoundingVolumeIntersects()) return false;
			mth::double4x4 ellipsoidWorldInv = mth::double4x4::Scaling(mth::double3(1) / m_ellipsoid.getRadii()) * m_ellipsoid.GetWorldMatrixInv();
			mth::double3x3 ellipsoidNoTranslateInv(ellipsoidWorldInv);
			mth::double4x4 transformMatrix = ellipsoidWorldInv * m_mesh.GetWorldMatrix();
			mth::double3 trVel = ellipsoidNoTranslateInv * (m_vEllipsoid - m_vMesh);	//transformed velocity
			double previousCollTime = m_collData.time;	//save previous collision so we know if collision happenes here
			for (unsigned i = 0; i < m_mesh.getTriangleCount(); i++)
			{
				mth::Triangle3Dd triangle(
					mth::Transform(transformMatrix, m_mesh.getTriangleVertex(i, 0)),
					mth::Transform(transformMatrix, m_mesh.getTriangleVertex(i, 1)),
					mth::Transform(transformMatrix, m_mesh.getTriangleVertex(i, 2)));
				//if won't get close to the plain to intersect, skip
				if (triangle.getPlain().Distance(trVel) > 1.0)
					continue;
				if (CollideWithFace(triangle, trVel))
					continue;
				if (CollideWithEdges(triangle, trVel))
					continue;
				if (CollideWithVertices(triangle, trVel))
					continue;
			}
			//no collision happened
			if (m_collData.time == previousCollTime)
				return false;
			//collision happened
			//transform normal back to original space
			m_collData.point = mth::Transform(m_ellipsoid.GetWorldMatrix(), m_collData.normal);
			m_collData.normal = (m_collData.normal * ellipsoidNoTranslateInv).Normalized();
			return true;
		}
	}
}