// ============================================================================

#include "violet/serialization/Serializer.h"

#include <cstring>

// ============================================================================

template <typename Derived>
uint32 Violet::Component::getStaticFlag()
{
	static const uint32 s_flag = getNextFlag();
	return s_flag;
}

// ----------------------------------------------------------------------------

template <typename ComponentType>
uint32 Violet::Component::gatherFlags()
{
	return ComponentType::getStaticFlag();
}

// ----------------------------------------------------------------------------

template <typename ComponentType, typename OtherComponentType, typename ... ComponentTypes>
uint32 Violet::Component::gatherFlags()
{
	return gatherFlags<ComponentType>() | gatherFlags<OtherComponentType, ComponentTypes...>();
}

// ============================================================================

template <typename Derived>
uint32 Violet::ComponentBase<Derived>::getStaticFlag()
{
	return Component::getStaticFlag<Derived>();
}

// ============================================================================

template <typename Derived>
Violet::Tag Violet::ComponentBase<Derived>::getTag() const
{
	return Derived::getStaticTag();
}

// ----------------------------------------------------------------------------

template <typename Derived>
uint32 Violet::ComponentBase<Derived>::getFlag() const
{
	return getStaticFlag();
}

// ----------------------------------------------------------------------------

template <typename Derived>
void Violet::ComponentBase<Derived>::save(Serializer & serializer) const
{
	auto componentSegment = serializer.createSegment(Derived::getStaticTag().asString().c_str());
	*componentSegment << *static_cast<const Derived *>(this);
}

// ============================================================================

template <typename Derived>
Violet::ComponentBase<Derived>::ComponentBase(const Handle entityId) :
	Component(entityId)
{
}

// ----------------------------------------------------------------------------

template <typename Derived>
Violet::ComponentBase<Derived>::ComponentBase(ComponentBase && other) :
	Component(std::move(other))
{
}

// ============================================================================
