// ============================================================================

#include "violet/input/component/MouseInputComponent.h"

#include "violet/serialization/Serializer.h"

using namespace Violet;

// ============================================================================

Tag MouseInputComponent::getStaticTag()
{
	return Tag('m', 'i', 'p', 't');
}

// ----------------------------------------------------------------------------

Thread MouseInputComponent::getStaticThread()
{
	return Thread::Any;
}

// ============================================================================

MouseInputComponent::MouseInputComponent(const Handle entityId, Deserializer & deserializer) :
	ComponentBase<MouseInputComponent>(entityId),
	m_mesh(deserializer)
{
}

// ----------------------------------------------------------------------------

MouseInputComponent::MouseInputComponent(const Handle entityId, Polygon && mesh) :
	ComponentBase<MouseInputComponent>(entityId),
	m_mesh(std::move(mesh))
{
}

// ----------------------------------------------------------------------------

MouseInputComponent::MouseInputComponent(MouseInputComponent && other) :
	ComponentBase<MouseInputComponent>(std::move(other)),
	m_mesh(std::move(other.m_mesh))
{
}

// ============================================================================

Serializer & Violet::operator<<(Serializer & serializer, const MouseInputComponent & component)
{
	return serializer << component.m_mesh;
}

// ----------------------------------------------------------------------------

Deserializer & Violet::operator>>(Deserializer & deserializer, MouseInputComponent & component)
{
	return deserializer >> component.m_mesh;
}

// ============================================================================
