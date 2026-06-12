#pragma once

#include <string>
#include <memory>

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define GLCheckError() (glGetError() == GL_NO_ERROR)

namespace Refraction::Common {

	// Nicer way of doing smart pointers, thanks Cherno (https://github.com/TheCherno/Hazel/blob/1feb70572fa87fa1c4ba784a2cfeada5b4a500db/Hazel/src/Hazel/Core/Base.h#L38-L44)
	///

	// Shared pointer wrapper
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename... Args>
	// Creates a new Ref (shared pointer)
	constexpr Ref<T> NewRef(Args&&... args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
	template<typename T, typename... Args>
	constexpr Ref<T> WrapRef(Args&&... args) {
		return std::shared_ptr<T>(std::forward<Args>(args)...);
	}

	// Unique pointer wrapper
	template<typename T>
	using URef = std::unique_ptr<T>;
	template<typename T, typename... Args>
	// Creates a new URef (unique pointer)
	constexpr URef<T> NewURef(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}

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
