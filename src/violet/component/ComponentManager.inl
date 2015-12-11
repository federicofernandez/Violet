// ============================================================================

#include "violet/template/TupleUtilities.h"

#include <algorithm>

// ============================================================================

template <bool is_const, typename... ComponentTypes>
Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator::Iterator(iterator_tuple iterators) :
	m_iterators(iterators),
	m_entityId()
{
}

// ----------------------------------------------------------------------------

template <bool is_const, typename... ComponentTypes>
typename Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator & Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator::operator++()
{
	m_entityId = m_entityId.isValid() ? EntityId(m_entityId.getId(), m_entityId.getVersion() + 1) : EntityId(0, 0);
	advance();
	return *this;
}

// ----------------------------------------------------------------------------

template <bool is_const, typename... ComponentTypes>
typename Violet::ComponentManager::View<is_const, ComponentTypes...>::component_tuple Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator::operator*()
{
	return std::forward_as_tuple(*Violet::get<ComponentPool::Iterator<ComponentTypes, is_const>>(m_iterators)...);
}

// ----------------------------------------------------------------------------

template <bool is_const, typename... ComponentTypes>
Violet::EntityId Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator::getEntityId() const
{
	return m_entityId;
}

// ----------------------------------------------------------------------------

template <bool is_const, typename... ComponentTypes>
bool Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator::operator==(const Iterator & other) const
{
	return m_entityId == other.m_entityId;
}

// ----------------------------------------------------------------------------

template <bool is_const, typename... ComponentTypes>
bool Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator::operator!=(const Iterator & other) const
{
	return !(*this == other);
}

// ============================================================================

template <bool is_const, typename... ComponentTypes>
void Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator::advance()
{
	uint32 count = 0;
	advance<0>(count);
}

// ----------------------------------------------------------------------------

template <bool is_const, typename... ComponentTypes>
template <uint32 Index>
void Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator::advance(uint32 & count)
{
	if (count != sizeof...(ComponentTypes))
	{
		std::get<Index>(m_iterators).advanceTo(m_entityId);
		if (std::get<Index>(m_iterators))
		{
			if (std::get<Index>(m_iterators)->getEntityId() != m_entityId)
			{
				count = 1;
				m_entityId = std::get<Index>(m_iterators)->getEntityId();
			}
			else
				++count;

			advance<(Index + 1) % sizeof...(ComponentTypes)>(count);
		}
		else
		{
			count = sizeof...(ComponentTypes);
			m_entityId = Violet::EntityId::ms_invalid;
		}
	}
}

// ============================================================================

template <bool is_const, typename... ComponentTypes>
Violet::ComponentManager::View<is_const, ComponentTypes...>::View(const ComponentManager & manager) :
	m_manager(manager)
{
}

// ----------------------------------------------------------------------------

template <bool is_const, typename... ComponentTypes>
typename Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator Violet::ComponentManager::View<is_const, ComponentTypes...>::begin()
{
	auto iterator = Iterator(std::make_tuple(m_manager.getPool<ComponentTypes>()->begin<ComponentTypes>()...));
	++iterator;
	return iterator;
}

// ----------------------------------------------------------------------------

template <bool is_const, typename... ComponentTypes>
typename Violet::ComponentManager::View<is_const, ComponentTypes...>::Iterator Violet::ComponentManager::View<is_const, ComponentTypes...>::end()
{
	return Iterator(std::make_tuple(m_manager.getPool<ComponentTypes>()->end<ComponentTypes>()...));
}

// ============================================================================

template <typename ComponentType, typename... Args>
ComponentType & Violet::ComponentManager::createComponent(const EntityId entityId, Args &&... args)
{
	return getPool<ComponentType>()->create<ComponentType, Args...>(entityId, std::forward<Args>(args)...);
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
Violet::ComponentPool * Violet::ComponentManager::getPool()
{
	return getPool(ComponentType::getStaticTag());
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
const Violet::ComponentPool * Violet::ComponentManager::getPool() const
{
	return getPool(ComponentType::getStaticTag());
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
bool Violet::ComponentManager::hasComponent(const EntityId entityId) const
{
	return getPool<ComponentType>()->has(entityId);
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
ComponentType * Violet::ComponentManager::getComponent(const EntityId entityId)
{
	return getPool<ComponentType>()->get<ComponentType>(entityId);
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
const ComponentType * Violet::ComponentManager::getComponent(const EntityId entityId) const
{
	return getPool<ComponentType>()->get<ComponentType>(entityId);
}

// ----------------------------------------------------------------------------

template <typename... ComponentTypes>
Violet::ComponentManager::View<true, ComponentTypes...> Violet::ComponentManager::getEntityView() const
{
	return View<true, ComponentTypes...>(*this);
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
bool Violet::ComponentManager::remove(const EntityId entityId)
{
	return getPool<ComponentType>()->remove(entityId);
}

// ============================================================================

template <typename ComponentType>
void Violet::ComponentManager::installComponent()
{
	installComponent(ComponentType::getStaticTag(), &ComponentManager::createPool<ComponentType>, &ComponentManager::createComponents<ComponentType>, &ComponentManager::savePool<ComponentType>, ComponentType::getStaticThread());
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
void Violet::ComponentManager::uninstallComponent()
{
	uninstallComponent(ComponentType::getStaticTag());
}

// ============================================================================

template <typename ComponentType>
static Violet::ComponentPool Violet::ComponentManager::createPool()
{
	return ComponentPool::create<ComponentType>();
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
void Violet::ComponentManager::createComponents(ComponentPool & pool, Deserializer & deserializer, const EntityId::StorageType version)
{
	pool.load<ComponentType>(deserializer, version);
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
uint32 Violet::ComponentManager::savePool(const ComponentPool & pool, Serializer & serializer, const std::vector<EntityId> & entityIds)
{
	return pool.save<ComponentType>(serializer, entityIds);
}

// ============================================================================
