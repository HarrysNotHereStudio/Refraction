#pragma once

#include <algorithm>

#include <Math/Common.h>

namespace Refraction::Math {
	class Vector3;

	class Vector4 {
	public:
		float x = 0;
		float y = 0;
		float z = 0;
		float w = 0;

		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
		Vector4(float n) : x(n), y(n), z(n), w(n) {};
		Vector4() : x(0), y(0), z(0), w(0) {};

		Vector4(const Vector4& v) {
			x = v.x;
			y = v.y;
			z = v.z;
			w = v.w;
		}

		Vector4& operator=(const Vector4& v2) {
			if (this != &v2) {
				x = v2.x;
				y = v2.y;
				z = v2.z;
				w = v2.w;
			}
			return *this;
		}

		Vector4 operator-() const {
			return Vector4(-x, -y, -z, -w);
		}

		Vector4 operator*(float n) const {
			return Vector4(x * n, y * n, z * n, w * n);
		}
		Vector4 operator*(Vector4 v2) const {
			return Vector4(x * v2.x, y * v2.y, z * v2.z, w * v2.w);
		}

		Vector4 operator+(Vector4 v2) const {
			return Vector4(x + v2.x, y + v2.y, z + v2.z, w + v2.w);
		}
		Vector4 operator-(Vector4 v2) const {
			return Vector4(x - v2.x, y - v2.y, z - v2.z, w - v2.w);
		}

		void operator+=(Vector4 v2) {
			x += v2.x;
			y += v2.y;
			z += v2.z;
			w += v2.w;
		}
		void operator-=(Vector4 v2) {
			x -= v2.x;
			y -= v2.y;
			z -= v2.z;
			w -= v2.w;
		}

		// Compatibility with smaller vectors
		///
		Vector4 operator*(Vector3 v2) const;
		Vector4 operator+(Vector3 v2) const;
		Vector4 operator-(Vector3 v2) const;

		void operator+=(Vector3 v2);
		void operator-=(Vector3 v2);

		bool operator==(const Vector4& v2) {
			return (x == v2.x) && (y == v2.y) && (z == v2.z) && (w == v2.w);
		}
		bool operator<(const Vector4& v2) {
			return (x < v2.x) && (y < v2.y) && (z < v2.z) && (w < v2.w);
		}
		bool operator>(const Vector4& v2) {
			return (x > v2.x) && (y > v2.y) && (z > v2.z) && (w > v2.w);
		}

		float Dot(const Vector4& v2) const;
		static float Distance(const Vector4 v1, const Vector4 v2);

		void Normalise();

		inline float Magnitude() const {
			auto tempX = x;
			auto tempY = y;
			auto tempZ = z;
			auto tempW = w;
			if (tempX < 0) tempX *= -1;
			if (tempY < 0) tempY *= -1;
			if (tempZ < 0) tempZ *= -1;
			if (tempW < 0) tempW *= -1;
			return tempX + tempY + tempZ + tempW;
		}

		inline Vector4 Normalised() const {
			Vector4 copy = Vector4(x, y, z, w);
			copy.Normalise();
			return copy;
		}
	};
}
