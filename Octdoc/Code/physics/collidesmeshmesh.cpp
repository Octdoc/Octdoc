#include "collidesmeshmesh.h"

namespace octdoc
{
	namespace physx
	{
		bool Collides_MeshMesh::BoundingVolumeIntersects()
		{
			return (
				m_mesh1.getBoundingVolume().position + m_mesh1.getBoundingVolume().size * 0.5 -
				m_mesh2.getBoundingVolume().position + m_mesh2.getBoundingVolume().size * 0.5
				).LengthSquare() <
				(m_mesh1.getBoundingVolume().size * 0.5).LengthSquare() +
				(m_mesh2.getBoundingVolume().size * 0.5).LengthSquare();
		}

		bool Collides_MeshMesh::VerticesHitFace(mth::Triangle3Dd& tri1, mth::Triangle3Dd& tri2, mth::double3 v)
		{
			bool collision = false;
			for (int i = 0; i < 3; i++)
			{
				double time = tri1.getPlain().DirectionalDistance(tri2.getVertex(i), v);
				if (time > 0 && time < m_collData.time)
				{
					mth::double3 point(tri2.getVertex(i) + v * time);
					if (tri1.IsPointOver(point))
					{
						m_collData.time = time;
						m_collData.normal = tri1.getPlain().getNormal();
						m_collData.point = point;
						collision = true;
					}
				}
			}
			return collision;
		}

		bool Collides_MeshMesh::EdgesHitEdges(mth::Triangle3Dd& tri1, mth::Triangle3Dd& tri2, mth::double3 v)
		{
			bool collision = false;

			for (int i1 = 0; i1 < 3; i1++)
			{
				for (int i2 = 0; i2 < 3; i2++)
				{
					mth::double3
						p1(tri1.getVertex(i1)),
						q1(tri1.getVertex(i1 % 3)),
						p2(tri2.getVertex(i2)),
						q2(tri2.getVertex(i2 % 3));
					mth::Plain3Dd distancePlain((p1 - q1).Cross(p2 - q2).Normalized(), 0);
					double d1 = distancePlain.Distance(p1);
					double d2 = distancePlain.Distance(p2);
					double distTaken = -distancePlain.getNormal().Dot(v);
					double time = (d2 - d1) / distTaken;
					if (time > 0 && time < m_collData.time)
					{
						//collided
						m_collData.time = time;
						//...
						collision = true;
					}
				}
			}

			return collision;
		}

		Collides_MeshMesh::Collides_MeshMesh(Collider_Mesh& c1, mth::double3 v1, Collider_Mesh& c2, mth::double3 v2, CollisionData& collisionData) :
			m_mesh1(c1),
			m_v1(v1),
			m_mesh2(c2),
			m_v2(v2),
			m_collData(collisionData) {}

		bool Collides_MeshMesh::Collides()
		{
			//if (!BoundingVolumeIntersects()) return false;
			mth::double4x4 transformMatrix = m_mesh1.GetWorldMatrix();
			mth::double3 trVel = m_v2 - m_v1;
			double prevTime = m_collData.time;
			for (int i1 = 0; i1 < m_mesh1.getTriangleCount(); i1++)
			{
				mth::Triangle3Dd tri1(
					mth::Transform(transformMatrix, m_mesh1.getTriangleVertex(i1, 0)),
					mth::Transform(transformMatrix, m_mesh1.getTriangleVertex(i1, 1)),
					mth::Transform(transformMatrix, m_mesh1.getTriangleVertex(i1, 2)));
				for (int i2 = 0; i2 < m_mesh2.getTriangleCount(); i2++)
				{
					mth::Triangle3Dd tri2(
						m_mesh2.getTriangleVertex(i2, 0),
						m_mesh2.getTriangleVertex(i2, 1),
						m_mesh2.getTriangleVertex(i2, 2));
					VerticesHitFace(tri1, tri2, trVel);
					if (VerticesHitFace(tri1, tri2, trVel)) m_collData.normal *= -1;
					EdgesHitEdges(tri1, tri2, trVel);
				}
			}
			if (m_collData.time == prevTime)
				return false;

			return true;
		}
	}
}