#ifndef RIGID_BODY_COMPONENT_H
#define RIGID_BODY_COMPONENT_H

#include "violet/core/component/Component.h"

#include "violet/core/math/Polygon.h"

#ifdef WIN32
#ifdef VIOLETPHYSICS_EXPORT
#define VIOLET_PHYSICS_API __declspec(dllexport)
#else
#define VIOLET_PHYSICS_API __declspec(dllimport)
#endif
#else
#define VIOLET_API
#endif

namespace Violet
{
	class Deserializer;
	class Serializer;

	class VIOLET_PHYSICS_API PhysicsComponent : public Component
	{
	public:

		static const char * getLabel();
	public:

		PhysicsComponent(const Entity & entity, Deserializer & deserializer);
		PhysicsComponent(PhysicsComponent && other);
		PhysicsComponent & operator=(PhysicsComponent && other);

	public:

		Polygon m_polygon;
		float m_mass;
		Vec2f m_velocity;
		Vec2f m_force;
		float m_momentOfInertia;
		float m_angularVelocity;
		float m_torque;
	};

	Serializer & operator<<(Serializer & serializer, const PhysicsComponent & component);
}

#endif