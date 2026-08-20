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
		// Nicer way of doing smart pointers (https://github.com/TheCherno/Hazel/blob/1feb70572fa87fa1c4ba784a2cfeada5b4a500db/Hazel/src/Hazel/Core/Base.h#L38-L44)
		///

		// Shared pointer wrapper
		template<typename T>
		using SRef = std::shared_ptr<T>;
		// Creates a new SRef (shared pointer)
		template<typename T, typename... Args>
		constexpr SRef<T> NewSRef(Args&&... args) {
			return std::make_shared<T>(std::forward<Args>(args)...);
		}

		// Unique pointer wrapper
		template<typename T>
		using URef = std::unique_ptr<T>;
		// Creates a new URef (unique pointer)
		template<typename T, typename... Args>
		constexpr URef<T> NewURef(Args&&... args) {
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

		// Custom pointer of a pointer object
		template<typename T>
		class Ref {
		public:
			// Construct empty reference
			Ref() : mPtr(nullptr) {}
			// Construct empty reference
			Ref(nullptr_t) : mPtr(nullptr) {}
			// Construct with reference
			Ref(T* ptr) {
				if (ptr) mPtr = &ptr;
			}
			Ref(const Ref& other) {
				mPtr = other.mPtr;
			}
			~Ref() {
				mPtr = nullptr;
			}

			inline Ref operator=(const Ref& other) {
				if (this != &other) {
					mPtr = other.mPtr;
				}
				return *this;
			}

			// Returns the raw pointer to the object
			inline T* Get() const noexcept { return *mPtr; }
			// Returns whether the reference is valid or not
			inline bool Valid() { return (mPtr != nullptr && *mPtr != nullptr); }

			inline T* operator->() const noexcept { return *mPtr; }
			inline T& operator*() const noexcept { return **mPtr; }

			// Simpler form of conditioning
			inline operator bool() { return Valid(); }
		private:
			T** mPtr = nullptr;
		};

		template<typename T, typename O>
		constexpr T* AsA(O&& object) {
			return dynamic_cast<T*>(object);
		}
		template<typename T, typename O>
		constexpr bool IsA(O&& object) {
			return dynamic_cast<T*>(object) != 0;
		}
		template<typename T, typename O>
		constexpr T* AsA(SRef<O> object) {
			return dynamic_cast<T*>(object.get());
		}
		template<typename T, typename O>
		constexpr bool IsA(SRef<O> object) {
			return dynamic_cast<T*>(object.get()) != 0;
		}
		template<typename T, typename O>
		constexpr T* AsA(Ref<O> object) {
			return dynamic_cast<T*>(object.Get());
		}
		template<typename T, typename O>
		constexpr bool IsA(Ref<O> object) {
			return dynamic_cast<T*>(object.Get()) != 0;
		}
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
