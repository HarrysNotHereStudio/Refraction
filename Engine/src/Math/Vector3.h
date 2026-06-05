#pragma once

#include <algorithm>

#include <Math/Common.h>

namespace Refraction::Math {
	class Vector3 {
	public:
		float x = 0;
		float y = 0;
		float z = 0;

		static inline Vector3 Front() { return Vector3(0, 0, 1); };
		static inline Vector3 Right() { return Vector3(1, 0, 0); };
		static inline Vector3 Up() { return Vector3(0, 1, 0); };
		static inline Vector3 Zero() { return Vector3(1); };
		static inline Vector3 One() { return Vector3(0); };

		Vector3(float x, float y, float z) : x(x), y(y), z(z) {};
		Vector3(int x, int y, int z) : x((float)x), y((float)y), z((float)z) {};
		Vector3(float n) : x(n), y(n), z(n) {};
		Vector3(int n) : x((float)n), y((float)n), z((float)n) {};
		Vector3() : x(0), y(0), z(0) {};

		Vector3(const Vector3& v) {
			x = v.x;
			y = v.y;
			z = v.z;
		}

		Vector3& operator=(const Vector3& v2) {
			if (this != &v2) {
				x = v2.x;
				y = v2.y;
				z = v2.z;
			}
			return *this;
		}

		Vector3 operator-() const {
			return Vector3(-x, -y, -z);
		}

		Vector3 operator*(float n) const {
			return Vector3(x * n, y * n, z * n);
		}
		Vector3 operator*(Vector3 v2) const {
			return Vector3(x * v2.x, y * v2.y, z * v2.z);
		}

		Vector3 operator+(Vector3 v2) const {
			return Vector3(x + v2.x, y + v2.y, z + v2.z);
		}
		Vector3 operator-(Vector3 v2) const {
			return Vector3(x - v2.x, y - v2.y, z - v2.z);
		}

		void operator+=(Vector3 v2) {
			x += v2.x;
			y += v2.y;
			z += v2.z;
		}
		void operator-=(Vector3 v2) {
			x -= v2.x;
			y -= v2.y;
			z -= v2.z;
		}

		bool operator==(const Vector3& v2) const = default;
		bool operator<(const Vector3& v2) const {
			return (x < v2.x) && (y < v2.y) && (z < v2.z);
		}
		bool operator>(const Vector3& v2) const {
			return (x > v2.x) && (y > v2.y) && (z > v2.z);
		}

		float Dot(const Vector3& v2) const;
		Vector3 Cross(const Vector3& v2) const;
		static float Distance(const Vector3 v1, const Vector3 v2);

		void Normalise();

		inline float Magnitude() const {
			auto tempX = x;
			auto tempY = y;
			auto tempZ = z;
			if (tempX < 0) tempX *= -1;
			if (tempY < 0) tempY *= -1;
			if (tempZ < 0) tempZ *= -1;
			return tempX + tempY + tempZ;
		}

		inline Vector3 Normalised() const {
			Vector3 copy = Vector3(x, y, z);
			copy.Normalise();
			return copy;
		}
	};
}
