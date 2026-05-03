#pragma once

#include <string>

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define GLCheckError() (glGetError() == GL_NO_ERROR)

namespace EngineConstants {
	static std::string ResourcePath;
}