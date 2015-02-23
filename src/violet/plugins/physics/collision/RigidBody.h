#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include "violet/core/math/Interval.h"
#include "violet/core/math/Polygon.h"
#include "violet/core/math/Vec2.h"


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
	class TransformComponent;
	class PhysicsComponent;

	class VIOLET_PHYSICS_API RigidBody
	{
	public:

		RigidBody(const TransformComponent & transform, const PhysicsComponent & physics);
		RigidBody(Vec2f && center, Polygon && polygon, float mass);

		const Vec2f & getCenter();
		const Vec2f & getVelocity();

		void translate(const Vec2f & translation);
		void rotate(float radians);
		void applyImpulse(Vec2f impulse, const Vec2f & location = Vec2f::ZERO);

		FloatInterval project(const Vec2f & axis) const;
		float maxRadius() const;
		void findIntersectionAxes(const RigidBody & other, std::vector<Vec2f> & axes) const;
		void findClosestVertices(const Vec2f & axis, const Vec2f & goal, std::vector<Vec2f> & vertices) const;

	private:

		Vec2f m_center;
		float m_rotation;
		Polygon m_polygon;
		float m_mass;
		Vec2f m_velocity;
		Vec2f m_force;
		float m_momentOfInertia;
		float m_angularVelocity;
		float m_torque;
	};
}

#endif