#pragma once

#include <string>
#include <stdexcept>
#include <memory>

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define GLCheckError() (glGetError() == GL_NO_ERROR)

namespace Refraction {
	namespace Common {
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

		template<typename T, typename O>
		constexpr T* AsA(O&& object) {
			return dynamic_cast<T*>(object);
		}
		template<typename T, typename O>
		constexpr bool IsA(O&& object) {
			return dynamic_cast<T*>(object) != 0;
		}
		template<typename T, typename O>
		constexpr T* AsA(Ref<O> object) {
			return dynamic_cast<T*>(object.get());
		}
		template<typename T, typename O>
		constexpr bool IsA(Ref<O> object) {
			return dynamic_cast<T*>(object.get()) != 0;
		}


	}

	class Log {
	public:
		static std::string GenerateTimestamp();
		static void SInfo(std::string message);
		static void SWarn(std::string message);
		static void SError(std::string message);

		static Log Render;
		static Log Physics;
		static Log Runtime;
		static Log Editor;

		Log() : mName("Refraction") {}
		Log(std::string name) : mName(name) {}

		void Info(std::string message);
		void Warn(std::string message);
		void Error(std::string message);
	protected:
		std::string mName;
	};
}
