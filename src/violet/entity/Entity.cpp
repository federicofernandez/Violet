// ============================================================================

#include "violet/component/Component.h"
#include "violet/entity/Entity.h"
#include "violet/input/component/KeyInputComponent.h"
#include "violet/scene/Scene.h"
#include "violet/serialization/Deserializer.h"

#include <iostream>

using namespace Violet;

// ============================================================================

void Violet::Entity::installComponent(const Tag tag, const ComponentFactory::Producer producer)
{
	const std::string label(tag.asString(), 4);
#ifdef _DEBUG
	assert(!ms_componentFactory.has(label));
#endif
	ms_componentFactory.assign(label, producer);
}

// ----------------------------------------------------------------------------

void Violet::Entity::uninstallComponent(const Tag tag)
{
	const std::string label(tag.asString(), 4);
#ifdef _DEBUG
	assert(ms_componentFactory.has(label));
#endif
	ms_componentFactory.remove(label);
}

// ============================================================================

Factory<std::string, void(Entity &, Deserializer &)> Entity::ms_componentFactory;

// ============================================================================

Entity::Entity(Scene & scene) :
	m_components(),
	m_children(),
	m_componentFlags(),
	m_scene(scene),
	m_parent()
{
}

// ----------------------------------------------------------------------------


Entity::Entity(Scene & scene, Deserializer & deserializer) :
	m_components(),
	m_children(),
	m_componentFlags(0),
	m_scene(scene),
	m_parent()
{
	auto entitySegment = deserializer.enterSegment("ntty");
	auto componentsSegment = entitySegment->enterSegment("cpnt");
	while (*componentsSegment)
	{
		const char * const label = componentsSegment->nextLabel();
		auto componentSegment = componentsSegment->enterSegment(label);
		ms_componentFactory.create(label, *this, *componentSegment);
	}

	auto childrenSegment = entitySegment->enterSegment("chld");
	while (*childrenSegment)
		addChild(make_unique_val<Entity>(scene, *childrenSegment));
}

// ----------------------------------------------------------------------------

Entity::~Entity()
{
}

// ----------------------------------------------------------------------------

Scene & Entity::getScene()
{
	return m_scene;
}

// ----------------------------------------------------------------------------

const Scene & Entity::getScene() const
{
	return m_scene;
}

// ----------------------------------------------------------------------------

Entity & Entity::addChild()
{
	return addChild(make_unique_val<Entity>(m_scene));
}

// ----------------------------------------------------------------------------

Entity & Entity::addChild(unique_val<Entity> && child)
{
	m_children.emplace_back(std::move(child));
	Entity & newChild = *m_children.back();
	newChild.m_parent = this;
	return newChild;
}

// ----------------------------------------------------------------------------

Entity & Entity::addChild(Deserializer & deserializer)
{
	return addChild(make_unique_val<Entity>(m_scene, deserializer));
}

// ----------------------------------------------------------------------------

std::vector<unique_val<Entity>> & Entity::getChildren()
{
	return m_children;
}

// ----------------------------------------------------------------------------

const std::vector<unique_val<Entity>> & Entity::getChildren() const
{
	return m_children;
}

// ----------------------------------------------------------------------------

lent_ptr<Entity> Entity::getChild(const uint32 index)
{
	return index < m_children.size() ? m_children[index].ptr() : nullptr;
}

// ----------------------------------------------------------------------------

lent_ptr<const Entity> Entity::getChild(const uint32 index) const
{
	return index < m_children.size() ? m_children[index].ptr() : nullptr;
}

// ----------------------------------------------------------------------------

bool Entity::removeChild(const uint32 index)
{
	if (index < m_children.size())
	{
		m_children.erase(m_children.begin() + index);
		return true;
	}
	return false;
}

// ----------------------------------------------------------------------------

uint32 Entity::getComponentFlags() const
{
	return m_componentFlags;
}

// ----------------------------------------------------------------------------

lent_ptr<Entity> Entity::getParent()
{
	return m_parent;
}

// ----------------------------------------------------------------------------

lent_ptr<const Entity> Entity::getParent() const
{
	return m_parent;
}

// ----------------------------------------------------------------------------

void Entity::removeFromParent()
{
	if (m_parent != nullptr)
	{
		auto & children = m_parent->getChildren();
		const auto & it = std::find_if(children.begin(), children.end(), [=](const unique_val<Entity> & child) { return child.ptr() == this; });
		if (it != children.end())
			m_parent->removeChild(std::distance(children.begin(), it));
	}
}

// ============================================================================
