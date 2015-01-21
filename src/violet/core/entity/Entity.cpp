#include "violet/core/entity/Entity.h"

using namespace Violet;

namespace EntityNamespace
{
	uint32 ms_id = 0;
}

using namespace EntityNamespace;

// ============================================================================

Entity::Entity() :
	m_id(ms_id++)
{
}

Entity::Entity(const Entity & other) :
	m_id(other.m_id)
{
}

// ============================================================================
