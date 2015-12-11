#ifndef VIOLET_LocalTransformComponent_H
#define VIOLET_LocalTransformComponent_H

#include "violet/component/Component.h"
#include "violet/task/Thread.h"

#include "violet/math/Matrix3.h"

namespace Violet
{
	class Deserializer;
	class Serializer;

	class VIOLET_API LocalTransformComponent : public ComponentBase<LocalTransformComponent>
	{
	public:

		static Tag getStaticTag();
		static Thread getStaticThread();

	public:

		LocalTransformComponent(EntityId entityId, Deserializer & deserializer);
		LocalTransformComponent(EntityId entityId, EntityId parentId, const Matrix3f & transform);
		LocalTransformComponent(LocalTransformComponent && other);

	public:

		EntityId m_parentId;
		Matrix3f m_transform;
	};

	VIOLET_API Deserializer & operator>>(Deserializer & deserializer, LocalTransformComponent & component);
	VIOLET_API Serializer & operator<<(Serializer & serializer, const LocalTransformComponent & component);
}

#endif