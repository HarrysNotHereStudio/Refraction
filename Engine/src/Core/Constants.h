#pragma once

#include <string>

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define GLCheckError() (glGetError() == GL_NO_ERROR)

namespace Refraction::Constants {
	static std::string ResourcePath("../../Resources/");

	static std::string GetResourcePath() { return std::string(ResourcePath); };
}

namespace Refraction::Enums {
	enum WindowInputState {
		VIEWPORT,
		GUI,
		NONE
	};
}