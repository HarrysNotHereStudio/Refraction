#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iterator>
#include <format>
#include <memory>

#include <glm/glm.hpp>
#include <Math/Vector3.h>
#include <Math/Matrices.h>

namespace fs = std::filesystem;


namespace Refraction::Utilities {
	extern std::string EngineWorkingDirectory;

	extern int RandomI(const int max, const int min = 0);
	extern float RandomF(const float max = 1, const float min = 0);

	// Produces a reasonably unique universal ID
	extern std::string GenerateUUID();

	// Returns a string of the contents of a file
	extern std::string ReadFile(const std::string& filename);

	extern std::vector<fs::directory_entry> GetFilesInFolder(fs::path folderPath);
	extern std::vector<fs::directory_entry> GetFilesOfExtInFolder(fs::path folderPath, std::string ext);
	extern fs::directory_entry GetFirstFileOfExtInFolder(fs::path folderPath, std::string ext);
	extern std::vector<fs::directory_entry> GetFoldersInFolder(fs::path folderPath);

	// Converts delta time (in ms) to a rate per second (e.g. FPS)
	extern std::string DeltaToRate(double deltaTime, int precision = -1);

	extern Refraction::Math::Vector3 BoolToVector3(bool pX, bool nX, bool pY, bool nY, bool pZ, bool nZ);
	extern inline glm::vec3 NativeToGLMVec3(Refraction::Math::Vector3 v) { return glm::vec3(v.x, v.y, v.z); };
	extern inline Refraction::Math::Vector3 GLMToNativeVec3(glm::vec3 v) { return Refraction::Math::Vector3(v.x, v.y, v.z); };
	extern glm::mat3 NativeToGLMMat3(Refraction::Math::Matrix3 m);
	extern glm::mat4 NativeToGLMMat4(Refraction::Math::Matrix4 m);
};

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

	// Unique pointer wrapper
	template<typename T>
	using URef = std::unique_ptr<T>;
	template<typename T, typename... Args>
	// Creates a new URef (unique pointer)
	constexpr URef<T> NewURef(Args&&... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}