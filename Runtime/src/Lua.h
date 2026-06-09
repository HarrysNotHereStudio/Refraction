#pragma once

#include "Common.h"

#pragma managed(push, off)
#include <sol/sol.hpp>
#pragma managed(pop)

extern "C" {
	static sol::state lua;

	Exported void InitLua();
	Exported void ParseLua(const char* source);
}
