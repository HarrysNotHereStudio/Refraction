#pragma once

#include <algorithm>
#include <string>

#include <Math/Common.h>

namespace Refraction::Math {
	class Vector3 {
	public:
		float x = 0;
		float y = 0;
		float z = 0;

		static Vector3 Front() { return Vector3(0, 0, -1); }
		static Vector3 Right() { return Vector3(1, 0, 0); }
		static Vector3 Up() { return Vector3(0, 1, 0); }
		static Vector3 X() { return Vector3(1, 0, 0); }
		static Vector3 Y() { return Vector3(0, 1, 0); }
		static Vector3 Z() { return Vector3(0, 0, 1); }
		static Vector3 Zero() { return Vector3(0); }
		static Vector3 One() { return Vector3(1); }

		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3(int x, int y, int z) : x((float)x), y((float)y), z((float)z) {}
		Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
		Vector3(float n) : x(n), y(n), z(n) {}
		Vector3(int n) : x((float)n), y((float)n), z((float)n) {}
		Vector3() : x(0), y(0), z(0) {}

		Vector3& operator=(const Vector3& v2) {
			if (this != &v2) { x = v2.x; y = v2.y; z = v2.z; }
			return *this;
		}

		Vector3 operator-() const { return Vector3(-x, -y, -z); }


		Vector3 operator+(Vector3 v2) const { return Vector3(x + v2.x, y + v2.y, z + v2.z); }
		Vector3 operator-(Vector3 v2) const { return Vector3(x - v2.x, y - v2.y, z - v2.z); }
		Vector3 operator*(Vector3 v2) const { return Vector3(x * v2.x, y * v2.y, z * v2.z); }
		Vector3 operator*(float n) const { return Vector3(x * n, y * n, z * n); }

		void operator+=(Vector3 v2) { x += v2.x; y += v2.y; z += v2.z; }
		void operator-=(Vector3 v2) { x -= v2.x; y -= v2.y; z -= v2.z; }
		void operator*=(Vector3 v2) { x *= v2.x; y *= v2.y; z *= v2.z; }
		void operator*=(float n) { x *= n; y *= n; z *= n; }

		bool operator==(const Vector3& v2) const = default;
		bool operator<(const Vector3& v2) const { return (x < v2.x) && (y < v2.y) && (z < v2.z); }
		bool operator>(const Vector3& v2) const { return (x > v2.x) && (y > v2.y) && (z > v2.z); }

		inline void Normalise() { (*this) *= 1.0f / sqrtf(Dot((*this))); }
		inline float Magnitude() const { return fabsf(x) + fabsf(y) + fabsf(z); }
		inline float Dot(const Vector3& v2) const { return x * v2.x + y * v2.y + z * v2.z; };
		inline float Distance(const Vector3& v2) const { return sqrtf(powf(x - v2.x, 2) + powf(y - v2.y, 2) + powf(z - v2.z, 2)); };
		inline Vector3 Cross(const Vector3& v2) const { return Vector3(y * v2.z - z * v2.y, z * v2.x - x * v2.z, x * v2.y - y * v2.x); };

		inline Vector3 Normalised() const {
			Vector3 copy = Vector3(x, y, z);
			copy.Normalise();
			return copy;
		}

		inline std::string ToString(bool pretty = true) const {
			if (pretty) {
				return std::string("x: " + std::to_string(x) + "\ny: " + std::to_string(y) + "\nz: " + std::to_string(z));
			} else {
				return std::string("{" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + "}");
			}
		}
	};
}
