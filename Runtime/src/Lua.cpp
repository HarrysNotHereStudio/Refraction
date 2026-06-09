#include <Core/Log.h>
#include <Engine.h>

#include "Lua.h"

void InitLua() {
	Refraction::RuntimeLog::Info("Initialising Lua...");
	lua.open_libraries(sol::lib::base);

	lua.script("print('hello moon!')");
}

void ParseLua(const char* source) {
	try {
		lua.script(source);
	} catch (sol::error e) {
		std::string err(e.what());
		Refraction::RuntimeLog::Info("Error while parsing Lua: " + err);
	}
}