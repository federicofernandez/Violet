#ifndef CPP_SCRIPT_H
#define CPP_SCRIPT_H

#include "violet/core/Defines.h"
#include "violet/core/script/Script.h"

#include "Windows.h"
#include <functional>

namespace Violet
{
	class VIOLET_API CppScript : public Script
	{
	public:

		static void install();

	public:

		CppScript(const char * filename);

	public:

		virtual ~CppScript() override;

		virtual void run(const char * procedure, const Entity & entity, AlterContext & context) const override;

	private:

		HMODULE m_lib;
		std::function<void(const Entity &)> m_proc;
	};
}

#endif