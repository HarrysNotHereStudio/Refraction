#pragma once

#include <algorithm>
#include <string>

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

		Vector2 operator+(Vector2 v2) const { return Vector2(x + v2.x, y + v2.y); }
		Vector2 operator-(Vector2 v2) const { return Vector2(x - v2.x, y - v2.y); }
		Vector2 operator*(Vector2 v2) const { return Vector2(x * v2.x, y * v2.y); }
		Vector2 operator*(float n) const { return Vector2(x * n, y * n); }

		void operator+=(Vector2 v2) { x += v2.x; y += v2.y; }
		void operator-=(Vector2 v2) { x -= v2.x; y -= v2.y; }
		void operator*=(Vector2 v2) { x *= v2.x; y *= v2.y; }
		void operator*=(float n) { x *= n; y *= n; }

		bool operator==(const Vector2& v2) const = default;
		bool operator<(const Vector2& v2) const { return (x < v2.x) && (y < v2.y); }
		bool operator>(const Vector2& v2) const { return (x > v2.x) && (y > v2.y); }

		inline void Normalise() { (*this) *= 1.0f / sqrtf(Dot((*this))); }
		inline float Magnitude() const { return fabsf(x) + fabsf(y); }
		inline float Dot(const Vector2& v2) const { return x * v2.x + y * v2.y; };
		inline float Distance(const Vector2& v2) const { return sqrtf(powf(x - v2.x, 2) + powf(y - v2.y, 2)); };

		inline Vector2 Normalised() const {
			Vector2 copy = Vector2(x, y);
			copy.Normalise();
			return copy;
		}

		inline std::string ToString(bool pretty = true) const {
			if (pretty) {
				return std::string("x: " + std::to_string(x) + "\ny: " + std::to_string(y));
			} else {
				return std::string("{" + std::to_string(x) + ", " + std::to_string(y) + "}");
			}
		}
	};
}