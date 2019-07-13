#pragma once

#include "collider.h"
#include "graphics/entity.h"

namespace octdoc
{
	namespace physx
	{
		class PhysicsObject
		{
			Collider::P m_physxObj;
			gfx::Entity::P m_gfxObj;

			mth::double3 m_force;
			mth::double3 m_velocity;
			mth::double3 m_centreOfMass;
			double m_massInv;

		public:
			PhysicsObject(Collider::P physxObj, gfx::Entity::P gfxObj);
			PhysicsObject(gfx::Graphics& graphics, hlp::ModelLoader& ml);

			void Update(double delta);
			void ApplyForce(mth::double3 force);
			void ApplyForce(mth::double3 force, mth::double3 position);

			inline double getMassInv() { return m_massInv; }
			inline Collider::P getCollider() { return m_physxObj; }
			inline gfx::Entity::P getEntity() { return m_gfxObj; }
		};
	}
}