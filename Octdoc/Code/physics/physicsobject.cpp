#include "physicsobject.h"

namespace octdoc
{
	namespace physx
	{
		PhysicsObject::PhysicsObject(Collider::P physxObj, gfx::Entity::P gfxObj) :
			m_physxObj(physxObj),
			m_gfxObj(gfxObj) {}
		PhysicsObject::PhysicsObject(gfx::Graphics& graphics, hlp::ModelLoader& ml)
		{
			m_physxObj = Collider::CreateP(ml);
			m_gfxObj = gfx::Entity::CreateP(graphics, ml);
		}
		void PhysicsObject::Update(double delta)
		{
			m_velocity += m_force * (m_massInv * delta);
			m_force = 0;
		}
		void PhysicsObject::ApplyForce(mth::double3 force)
		{
			m_force += force;
		}
		void PhysicsObject::ApplyForce(mth::double3 force, mth::double3 position)
		{
		}
	}
}