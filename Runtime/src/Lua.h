#pragma once

#include "Common.h"

#pragma managed(push, off)
#include <sol/sol.hpp>
#pragma managed(pop)

extern "C" {
	static sol::state lua;
	Exported void InitLua() {
		RuntimeLog::Info("Initialising Lua...");
		lua.open_libraries(sol::lib::base);

		lua.script("print('hello moon!')");
	}

	Exported void ParseLua(const char* source) {
		try {
			lua.script(source);
		} catch (sol::error e) {
			std::string err(e.what());
			RuntimeLog::Info("Error while parsing Lua: " + err);
		}
	}
}
