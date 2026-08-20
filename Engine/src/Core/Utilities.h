#pragma once

#include <string>

#include <glm/glm.hpp>

#include <Math/Vector.h>
#include <Math/Matrix.h>

namespace Refraction::Utilities {
	extern std::string EngineWorkingDirectory;

	extern int RandomI(const int max, const int min = 0);
	extern float RandomF(const float max = 1, const float min = 0);

	// Converts delta time (in ms) to a rate per second (e.g. FPS)
	extern std::string DeltaToRate(double deltaTime, int precision = -1);

	extern Refraction::Math::Vector3 BoolToVector3(bool pX, bool nX, bool pY, bool nY, bool pZ, bool nZ);
	extern inline glm::vec3 NativeToGLMVec3(Refraction::Math::Vector3 v) { return glm::vec3(v.x, v.y, v.z); };
	extern inline Refraction::Math::Vector3 GLMToNativeVec3(glm::vec3 v) { return Refraction::Math::Vector3(v.x, v.y, v.z); };
	extern glm::mat3 NativeToGLMMat3(Refraction::Math::Matrix3 m);
	extern Refraction::Math::Matrix3 GLMToNativeMat3(glm::mat3 m);
	extern glm::mat4 NativeToGLMMat4(Refraction::Math::Matrix4 m);
	extern Refraction::Math::Matrix4 GLMToNativeMat4(glm::mat4 m);
};
