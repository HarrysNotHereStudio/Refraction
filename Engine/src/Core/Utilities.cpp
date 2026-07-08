#include <random>

#include "Utilities.h"

std::random_device randDevice;
std::mt19937 randGen(randDevice());

namespace Refraction::Utilities {
	int RandomI(const int max, const int min) {
		std::uniform_int_distribution<> dist(min, max);
		return dist(randGen);
	}

	float RandomF(const float max, const float min) {
		std::uniform_real_distribution<> dist(min, max);
		return (float)dist(randGen);
	}


	// Math utilities
	///

	std::string DeltaToRate(double deltaTime, int precision) {
		double fps = 1.0 / deltaTime;
		if (precision < 0) return std::to_string(fps);
		if (precision == 0) return std::to_string(trunc(fps));

		std::string fpsString = std::to_string(fps);
		return fpsString.substr(0, fpsString.find(".") + precision + 1);
	}

	Math::Vector3 BoolToVector3(bool pX, bool nX, bool pY, bool nY, bool pZ, bool nZ) {
		return Math::Vector3(pX - nX, pY - nY, pZ - nZ);
	}

	glm::mat3 NativeToGLMMat3(Math::Matrix3 m) {
		return glm::mat3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);
	}

	Refraction::Math::Matrix3 GLMToNativeMat3(glm::mat3 m) {
		Refraction::Math::Matrix3 newMat;
		for (size_t row = 0; row < 3; row++) {
			for (size_t col = 0; col < 3; col++) {
				newMat[row][col] = m[(int)row][(int)col];
			}
		}
		return newMat;
	}

	glm::mat4 NativeToGLMMat4(Math::Matrix4 m) {
		return glm::mat4(m[0][0], m[0][1], m[0][2], m[0][3], m[1][0], m[1][1], m[1][2], m[1][3], m[2][0], m[2][1], m[2][2], m[2][3], m[3][0], m[3][1], m[3][2], m[3][3]);
	}

	Refraction::Math::Matrix4 GLMToNativeMat4(glm::mat4 m) {
		Refraction::Math::Matrix4 newMat;
		for (size_t row = 0; row < 4; row++) {
			for (size_t col = 0; col < 4; col++) {
				newMat[row][col] = m[(int)row][(int)col];
			}
		}
		return newMat;
	}
}
