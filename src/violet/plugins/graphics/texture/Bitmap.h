#ifndef BITMAP_H
#define BITMAP_H

#include "violet/core/utility/ResourceCache.h"

#include <memory>
#include <iosfwd>

namespace Violet
{
	class Bitmap
	{
	public:
		
		static std::unique_ptr<Bitmap> load(const char * filename);
		static std::unique_ptr<Bitmap> load(std::istream & stream);
		typedef ResourceCache<Bitmap, &Bitmap::load> Cache;
		static Cache & getCache();

	private:

		uint32 m_texture;
	};
}

#endif