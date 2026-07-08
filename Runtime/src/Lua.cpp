#include <Core/Common.h>

#include "Lua.h"

void InitLua() {
	Refraction::Log::Runtime.Info("Initialising Lua...");
	lua.open_libraries(sol::lib::base);

	lua.script("print('hello moon!')");
}

void ParseLua(const char* source) {
	try {
		lua.script(source);
	} catch (sol::error e) {
		std::string err(e.what());
		Refraction::Log::Runtime.Info("Error while parsing Lua: " + err);
	}
}