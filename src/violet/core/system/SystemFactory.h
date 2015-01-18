#ifndef SYSTEM_FACTORY_H
#define SYSTEM_FACTORY_H

#include "violet/core/utility/Factory.h"

#include <memory>

namespace Violet
{
	class Deserializer;
	class System;
	class SystemFactory : public Factory<const char *, std::unique_ptr<System>(Deserializer &)> {};
}

#endif