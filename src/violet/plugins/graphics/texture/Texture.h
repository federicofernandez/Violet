#ifndef TEXTURE_H
#define TEXTURE_H

#include "violet/core/Defines.h"

#include <memory>

namespace Violet
{
	class Texture
	{
	public:

		static void bind(const Texture & texture);
		static void unbind();

	public:

		Texture(uint32 width, uint32 height, const void * data, uint32 format);
		Texture(Texture && other);
		~Texture();

	private:

		Texture(const Texture & other) = delete;
		Texture & operator=(const Texture & other) = delete;

	private:

		uint32 m_handle;
	};
}

#endif