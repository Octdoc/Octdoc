#include "boundingvolume.h"

namespace octdoc
{
	namespace physx
	{
		bool Intersects_PointCuboid(mth::float3 p1, mth::float3 p2min, mth::float3 p2max)
		{
			return
				p2min.x < p1.x && p1.x < p2max.x &&
				p2min.y < p1.y && p1.y < p2max.y &&
				p2min.z < p1.z && p1.z < p2max.z;
		}
		bool Intersects_PointSphere(mth::float3 p1, mth::float3 p2, float r)
		{
			return (p1 - p2).LengthSquare() < r*r;
		}

		bool Intersects_CuboidCuboid(BV_Cuboid& h1, BV_Cuboid& h2)
		{
			return Intersects_PointCuboid(h1.position, h2.position - h1.size, h2.position + h2.size);
		}
		bool Intersects_CuboidSphere(BV_Cuboid& h1, BV_Sphere& h2)
		{
			mth::float3 point;
			point.x = fmaxf(h1.position.x, fminf(h2.position.x, h1.position.x + h1.size.x));
			point.y = fmaxf(h1.position.y, fminf(h2.position.y, h1.position.y + h1.size.y));
			point.z = fmaxf(h1.position.z, fminf(h2.position.z, h1.position.z + h1.size.z));
			return Intersects_PointSphere(point, h2.position, h2.radius);
		}
		bool Intersects_SphereSphere(BV_Sphere& h1, BV_Sphere& h2)
		{
			return Intersects_PointSphere(h1.position, h2.position, h1.radius + h2.radius);
		}

		BV_Cuboid::BV_Cuboid() :
			position(),
			size() {}
		BV_Cuboid::BV_Cuboid(mth::float3 position, mth::float3 size) :
			position(position),
			size(size) {}
		BV_Cuboid::U BV_Cuboid::CreateU()
		{
			return std::make_unique<BV_Cuboid>();
		}
		BV_Cuboid::U BV_Cuboid::CreateU(mth::float3 position, mth::float3 size)
		{
			return std::make_unique<BV_Cuboid>(position, size);
		}
		BV_Cuboid::P BV_Cuboid::CreateP()
		{
			return std::make_shared<BV_Cuboid>();
		}
		BV_Cuboid::P BV_Cuboid::CreateP(mth::float3 position, mth::float3 size)
		{
			return std::make_shared<BV_Cuboid>(position, size);
		}
		bool BV_Cuboid::Intersects(BoundingVolume& other)
		{
			return other.Intersects(*this);
		}
		bool BV_Cuboid::Intersects(BV_Cuboid& other)
		{
			return Intersects_CuboidCuboid(*this, other);
		}
		bool BV_Cuboid::Intersects(BV_Sphere& other)
		{
			return Intersects_CuboidSphere(*this, other);
		}

		BV_Sphere::BV_Sphere() :
			position(), 
			radius(0.0f) {}
		BV_Sphere::BV_Sphere(mth::float3 position, float radius) :
			position(position),
			radius(radius) {}
		BV_Sphere::U BV_Sphere::CreateU()
		{
			return std::make_unique<BV_Sphere>();
		}
		BV_Sphere::U BV_Sphere::CreateU(mth::float3 position, float radius)
		{
			return std::make_unique<BV_Sphere>(position, radius);
		}
		BV_Sphere::P BV_Sphere::CreateP()
		{
			return std::make_shared<BV_Sphere>();
		}
		BV_Sphere::P BV_Sphere::CreateP(mth::float3 position, float radius)
		{
			return std::make_shared<BV_Sphere>(position, radius);
		}
		bool BV_Sphere::Intersects(BoundingVolume& other)
		{
			return other.Intersects(*this);
		}
		bool BV_Sphere::Intersects(BV_Cuboid& other)
		{
			return Intersects_CuboidSphere(other, *this);
		}
		bool BV_Sphere::Intersects(BV_Sphere& other)
		{
			return Intersects_SphereSphere(*this, other);
		}

		bool Intersects(BoundingVolume& bv1, BoundingVolume& bv2)
		{
			return bv1.Intersects(bv2);
		}
	}
}