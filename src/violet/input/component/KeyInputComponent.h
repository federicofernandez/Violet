#ifndef VIOLET_KeyInputComponent_H
#define VIOLET_KeyInputComponent_H

#include "violet/component/Component.h"
#include "violet/task/Thread.h"

namespace Violet
{
	class ComponentDeserializer;
	class Serializer;

	class VIOLET_API KeyInputComponent : public ComponentBase<KeyInputComponent>
	{
	public:

		static Tag getStaticTag();
		static Thread getStaticThread();

	public:

		KeyInputComponent(EntityId entityId);
		KeyInputComponent(EntityId entityId, ComponentDeserializer & deserializer);
		KeyInputComponent(KeyInputComponent && other);
	};

	VIOLET_API Serializer & operator<<(Serializer & serializer, const KeyInputComponent & component);
	VIOLET_API ComponentDeserializer & operator>>(ComponentDeserializer & deserializer, KeyInputComponent & component);
}

#endif