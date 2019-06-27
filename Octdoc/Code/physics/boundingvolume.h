#pragma once

#include "helpers/helpers.h"
#include "linalg.h"

namespace octdoc
{
	namespace physx
	{
		bool Intersects_PointCuboid(mth::double3 p1, mth::double3 p2min, mth::double3 p2max);
		bool Intersects_PointSphere(mth::double3 p1, mth::double3 p2, double r);

		class BV_Cuboid;
		class BV_Sphere;

		class BoundingVolume
		{
			SMART_PTR(BoundingVolume)
		public:
			enum Type :unsigned
			{
				NO_TYPE = 0,
				CUBOID = 1,
				SPHERE
			};

		public:
			virtual bool Intersects(BoundingVolume& other) = 0;
			virtual bool Intersects(BV_Cuboid& other) = 0;
			virtual bool Intersects(BV_Sphere& other) = 0;
		};

		class BV_Cuboid :public BoundingVolume
		{
			SMART_PTR(BV_Cuboid)
		public:
			mth::double3 position;
			mth::double3 size;

		public:
			BV_Cuboid();
			BV_Cuboid(mth::double3 position, mth::double3 size);
			static BV_Cuboid::U CreateU();
			static BV_Cuboid::U CreateU(mth::double3 position, mth::double3 size);
			static BV_Cuboid::P CreateP();
			static BV_Cuboid::P CreateP(mth::double3 position, mth::double3 size);
			virtual bool Intersects(BoundingVolume& other) override;
			virtual bool Intersects(BV_Cuboid& other) override;
			virtual bool Intersects(BV_Sphere& other) override;
		};

		class BV_Sphere :public BoundingVolume
		{
			SMART_PTR(BV_Sphere)
		public:
			mth::double3 position;
			double radius;

		public:
			BV_Sphere();
			BV_Sphere(mth::double3 position, double radius);
			static BV_Sphere::U CreateU();
			static BV_Sphere::U CreateU(mth::double3 position, double radius);
			static BV_Sphere::P CreateP();
			static BV_Sphere::P CreateP(mth::double3 position, double radius);
			virtual bool Intersects(BoundingVolume& other) override;
			virtual bool Intersects(BV_Cuboid& other) override;
			virtual bool Intersects(BV_Sphere& other) override;
		};

		bool Intersects(BoundingVolume& bv1, BoundingVolume& bv2);
	}
}