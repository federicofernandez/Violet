#ifndef VIOLET_TextComponent_H
#define VIOLET_TextComponent_H

#include "violet/component/Component.h"
#include "violet/graphics/component/RenderComponentData.h"
#include "violet/task/Thread.h"

#include <string>
#include <memory>

namespace Violet
{
	class Deserializer;
	class Font;
	class Serializer;

	class VIOLET_API TextComponent : public ComponentBase<TextComponent>, public RenderComponentData
	{
	public:

		static Tag getStaticTag();
		static Thread getStaticThread();

	public:

		TextComponent(Handle entityId, Deserializer & deserializer);
		TextComponent(TextComponent && other);

	private:

		TextComponent(const TextComponent &) = delete;
		TextComponent & operator=(const TextComponent &) = delete;

	public:

		std::string m_text;
		std::shared_ptr<Font> m_font;
	};

	VIOLET_API Deserializer & operator>>(Deserializer & deserializer, TextComponent & component);
	VIOLET_API Serializer & operator<<(Serializer & serializer, const TextComponent & component);
}

#endif