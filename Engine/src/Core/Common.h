#pragma once

#include <vector>
#include <string>
#include <functional>
#include <stdexcept>
#include <memory>

#define INVALID_UNIFORM_LOCATION 0xffffffff
#define GLCheckError() (glGetError() == GL_NO_ERROR)

namespace Refraction {
	namespace Common {
		// Shared pointer wrapper
		template<typename T>
		using Shared = std::shared_ptr<T>;
		// Creates a new Shared (shared pointer)
		template<typename T, typename... Args>
		constexpr Shared<T> NewShared(Args&&... args) {
			return std::make_shared<T>(std::forward<Args>(args)...);
		}

		// Weak pointer wrapper
		template<typename T>
		using Ref = std::weak_ptr<T>;
		// Creates a new Ref (weak pointer)
		template<typename T, typename... Args>
		constexpr Ref<T> NewRef(Shared<T> shared) {
			return std::weak_ptr<T>(shared);
		}

		// Unique pointer wrapper
		template<typename T>
		using URef = std::unique_ptr<T>;
		// Creates a new URef (unique pointer)
		template<typename T, typename... Args>
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
		constexpr T* AsA(Shared<O> object) {
			return dynamic_cast<T*>(object.get());
		}
		template<typename T, typename O>
		constexpr bool IsA(Shared<O> object) {
			return dynamic_cast<T*>(object.get()) != 0;
		}

		template <typename Base, typename Target>
		concept DerivesFrom = std::is_base_of<Base, Target>::value;

		class RuntimeError : public std::runtime_error {
		public:
			RuntimeError(std::string msg);
		};
	}

	class Log {
	public:
		struct Colour {
			int R, G, B;
			bool operator==(const Colour& other) const {
				return (R == other.R) && (G == other.G) && (B == other.B);
			}
		};
		// Takes the colour, the body, and whether to put this on a new line
		typedef std::function<void(Colour, std::string, bool)> LogCallback;

		static std::string GenerateTimestamp();
		static void SInfo(std::string message);
		static void SWarn(std::string message);
		static void SError(std::string message);
		static void AddLogCallback(LogCallback callback) { Callbacks.push_back(callback); }
		static void InitConsoleLog();

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
		static std::vector<LogCallback> Callbacks;
		std::string mName;
	private:
		static void GenerateLog(std::string logName, std::string message, std::string logType, Colour printColour, bool printStack = false, Colour typeColour = { 0,0,0 });
	};

}
