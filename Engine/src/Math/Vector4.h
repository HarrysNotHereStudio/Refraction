#pragma once

#include <algorithm>
#include <string>

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


		Vector4 operator+(Vector4 v2) const { return Vector4(x + v2.x, y + v2.y, z + v2.z, w + v2.w); }
		Vector4 operator-(Vector4 v2) const { return Vector4(x - v2.x, y - v2.y, z - v2.z, w - v2.w); }
		Vector4 operator*(Vector4 v2) const { return Vector4(x * v2.x, y * v2.y, z * v2.z, w * v2.w); }
		Vector4 operator*(float n) const { return Vector4(x * n, y * n, z * n, w * n); }

		void operator+=(Vector4 v2) { x += v2.x; y += v2.y; z += v2.z; w += v2.w; }
		void operator-=(Vector4 v2) { x -= v2.x; y -= v2.y; z -= v2.z; w -= v2.w; }
		void operator*=(Vector4 v2) { x *= v2.x; y *= v2.y; z *= v2.z; w *= v2.w; }
		void operator*=(float n) { x *= n; y *= n; z *= n; w *= n; }

		bool operator==(const Vector4& v2) const = default;
		bool operator<(const Vector4& v2) const { return (x < v2.x) && (y < v2.y) && (z < v2.z) && (w < v2.w); }
		bool operator>(const Vector4& v2) const { return (x > v2.x) && (y > v2.y) && (z > v2.z) && (w > v2.w); }

		// Compatibility with smaller vectors
		///
		Vector4 operator*(Vector3 v2) const;
		Vector4 operator+(Vector3 v2) const;
		Vector4 operator-(Vector3 v2) const;

		void operator+=(Vector3 v2);
		void operator-=(Vector3 v2);

		inline void Normalise() { (*this) *= 1.0f / sqrtf(Dot((*this))); }
		inline float Magnitude() const { return fabsf(x) + fabsf(y) + fabsf(z) + fabsf(w); }
		inline float Dot(const Vector4& v2) const { return x * v2.x + y * v2.y + z * v2.z + w * v2.w; };
		inline float Distance(const Vector4& v2) const { return sqrtf(powf(x - v2.x, 2) + powf(y - v2.y, 2) + powf(z - v2.z, 2) + powf(w - v2.w, 2)); };

		inline Vector4 Normalised() const {
			Vector4 copy = Vector4(x, y, z, w);
			copy.Normalise();
			return copy;
		}

		inline std::string ToString(PrintFormatArgs fmtArgs = PrintFormatArgs()) const {
			std::string xStr = fmtArgs.AsInt ? std::to_string((int)x) : std::to_string(x);
			std::string yStr = fmtArgs.AsInt ? std::to_string((int)y) : std::to_string(y);
			std::string zStr = fmtArgs.AsInt ? std::to_string((int)z) : std::to_string(z);
			std::string wStr = fmtArgs.AsInt ? std::to_string((int)w) : std::to_string(w);
			if (fmtArgs.Pretty) {
				return std::string("x: " + xStr + "\ny: " + yStr + "\nz: " + zStr + "\nw: " + wStr);
			} else {
				return std::string("{" + xStr + ", " + yStr + ", " + zStr + ", " + wStr + "}");
			}
		}
	};
}
