#include "violet/core/transform/TransformComponent.h"

#include "violet/core/serialization/Serializer.h"

using namespace Violet;

const char * TransformComponent::getLabel()
{
	return "tsfm";
}

TransformComponent::TransformComponent(const Entity & entity) :
	Component(entity),
	m_position(),
	m_rotation()
{
}

TransformComponent::TransformComponent(const Entity & entity, Deserializer & deserializer) :
	Component(entity),
	m_position(deserializer),
	m_rotation(0)
{
}

TransformComponent::TransformComponent(const Entity & entity, Vec2f position, float rotation) :
	Component(entity),
	m_position(position),
	m_rotation(rotation)
{
}

TransformComponent::TransformComponent(TransformComponent && other) :
	Component(std::move(other)),
	m_position(std::move(other.m_position)),
	m_rotation(other.m_rotation)
{
}

TransformComponent & TransformComponent::operator=(TransformComponent && other)
{
	m_position = std::move(other.m_position);
	m_rotation = other.m_rotation;
	return *this;
}

Serializer & Violet::operator<<(Serializer & serializer, const TransformComponent & component)
{
	serializer << component.m_position;
	return serializer;
}
