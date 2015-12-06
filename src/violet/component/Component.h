#ifndef VIOLET_Component_H
#define VIOLET_Component_H

#include "violet/utility/Tag.h"
#include "violet/handle/Handle.h"

#include <utility>

namespace Violet
{
	class Serializer;

	class VIOLET_API Component
	{
	public:

		template <typename ComponentType>
		static uint32 gatherFlags();
		template <typename ComponentType, typename OtherComponentType, typename ... ComponentTypes>
		static uint32 gatherFlags();

	protected:

		template <typename Derived>
		static uint32 getStaticFlag();

	private:

		static uint32 getNextFlag();

	public:

		Handle getEntityId() const;

		virtual Tag getTag() const = 0;
		virtual uint32 getFlag() const = 0;
		virtual void save(Serializer & serializer) const = 0;

		virtual ~Component() = 0;

	protected:

		Component(Handle entityId);

		Component(const Component &) = delete;
		Component & operator=(const Component &) = delete;

		Component(Component &&);

	protected:

		Handle m_entityId;
	};

	template <typename Derived>
	class ComponentBase : public Component
	{
	public:

		static uint32 getStaticFlag();

	public:

		virtual Tag getTag() const override;
		virtual uint32 getFlag() const override;
		virtual void save(Serializer & serializer) const override;

	protected:

		ComponentBase(Handle entityId);
		ComponentBase(ComponentBase && other);
	};
}

#include "violet/component/Component.inl"

#endif
