#pragma once

#include <Math/Common.h>
#include <Math/Vector2.h>

namespace Refraction::Math {
	class Frustum {
	public:
		int w, h;
		float fovY, zNear, zFar;

		// Create a Frustum with the given width, height, FOV and Z range
		Frustum(int w, int h, float fovY, float zNear, float zFar) : w(std::max(w, 0)), h(std::max(h, 0)), fovY(std::max(fovY, 10.0f)), zNear(std::max(zNear, 0.0f)), zFar(std::max(zFar, 0.0f)) {};
		// Create a Frustum with the given width, height, FOV and Z range
		Frustum(Vector2 size, float fovY, float zNear, float zFar) : Frustum((int)size.x, (int)size.y, fovY, zNear, zFar) {};

		inline bool operator==(const Frustum& other) const {
			return (w == other.w) && (h == other.h) && (zNear == other.zNear) && (zFar == other.zFar);
		}

		inline float AspectRatio() const { return w / (float)h; }

		inline std::string ToString(PrintFormatArgs fmtArgs = PrintFormatArgs()) const {
			if (fmtArgs.Pretty) {
				return std::string("w: " + std::to_string(w) + "\nh: " + std::to_string(h) + "\nfovY: " + std::to_string(fovY) + "\nzNear: " + std::to_string(zNear) + "\nzFar: " + std::to_string(zFar));
			} else {
				return std::string("{" + std::to_string(w) + ", " + std::to_string(h) + ", " + std::to_string(fovY) + ", " + std::to_string(zNear) + ", " + std::to_string(zFar) + "}");
			}
		}
	};
}
