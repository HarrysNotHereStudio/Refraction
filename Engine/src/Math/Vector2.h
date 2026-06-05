#pragma once

#include <algorithm>

#include <Math/Common.h>

namespace Refraction::Math {
	class Vector2 {
	public:
		float x = 0;
		float y = 0;

		Vector2(float x, float y) : x(x), y(y) {};
		Vector2(float n) : x(n), y(n) {};
		Vector2() : x(0), y(0) {};

		Vector2(const Vector2& v) {
			x = v.x;
			y = v.y;
		}

		Vector2& operator=(const Vector2& v2) {
			if (this != &v2) {
				x = v2.x;
				y = v2.y;
			}
			return *this;
		}

		Vector2 operator-() {
			return Vector2(-x, -y);
		}

		Vector2 operator*(float n) {
			return Vector2(x * n, y * n);
		}
		Vector2 operator*(Vector2 v) {
			return Vector2(x * v.x, y * v.y);
		}

		Vector2 operator+(Vector2 v2) {
			return Vector2(x + v2.x, y + v2.y);
		}
		Vector2 operator-(Vector2 v2) {
			return Vector2(x - v2.x, y - v2.y);
		}

		void operator+=(Vector2 v2) {
			x += v2.x;
			y += v2.y;
		}
		void operator-=(Vector2 v2) {
			x -= v2.x;
			y -= v2.y;
		}

		bool operator==(const Vector2& v2) const = default;
		bool operator<(const Vector2& v2) const {
			return (x < v2.x) && (y < v2.y);
		}
		bool operator>(const Vector2& v2) const {
			return (x > v2.x) && (y > v2.y);
		}

		static float Dot(Vector2 v1, Vector2 v2);
		static float Distance(const Vector2 v1, const Vector2 v2);

		void Normalise();

		inline float Magnitude() const {
			auto tempX = x;
			auto tempY = y;
			if (tempX < 0) tempX *= -1;
			if (tempY < 0) tempY *= -1;
			return tempX + tempY;
		}

		inline Vector2 Normalised() const {
			Vector2 copy = Vector2(x, y);
			copy.Normalise();
			return copy;
		}
	};
}