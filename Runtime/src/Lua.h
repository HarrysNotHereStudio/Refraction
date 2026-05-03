#pragma once

#include "Common.h"

#pragma managed(push, off)
#include <sol/sol.hpp>
#pragma managed(pop)

extern "C" {
	static sol::state lua;
	Exported void InitLua() {
		std::cout << "initialising lua\n";
		lua.open_libraries(sol::lib::base);

		lua.script("print('Hello, moon!')");
	}

	Exported void ParseLua(const char* source) {
		lua.script(source);
	}
}
