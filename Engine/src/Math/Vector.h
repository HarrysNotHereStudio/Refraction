#pragma once

#include <stdexcept>
#include <string>

#include <Math/Common.h>

namespace Refraction::Math {
	class Vector2 {
	public:
		float x = 0;
		float y = 0;

		Vector2(float x, float y) : x(x), y(y) {};
		Vector2(int x, int y) : Vector2((float)x, (float)y) {};
		Vector2(float n) : x(n), y(n) {};
		Vector2(int n) : Vector2((float)n) {};
		Vector2() : x(0), y(0) {};

		Vector2(const Vector2& v) {
			x = v.x;
			y = v.y;
		}

		inline Vector2& operator=(const Vector2& v2) {
			if (this != &v2) {
				x = v2.x;
				y = v2.y;
			}
			return *this;
		}

		inline Vector2 operator-() {
			return Vector2(-x, -y);
		}

		inline Vector2 operator+(Vector2 v2) const { return Vector2(x + v2.x, y + v2.y); }
		inline Vector2 operator-(Vector2 v2) const { return Vector2(x - v2.x, y - v2.y); }
		inline Vector2 operator*(Vector2 v2) const { return Vector2(x * v2.x, y * v2.y); }
		inline Vector2 operator*(float n) const { return Vector2(x * n, y * n); }

		inline void operator+=(Vector2 v2) { x += v2.x; y += v2.y; }
		inline void operator-=(Vector2 v2) { x -= v2.x; y -= v2.y; }
		inline void operator*=(Vector2 v2) { x *= v2.x; y *= v2.y; }
		inline void operator*=(float n) { x *= n; y *= n; }

		bool operator==(const Vector2& v2) const = default;
		inline bool operator<(const Vector2& v2) const { return (x < v2.x) && (y < v2.y); }
		inline bool operator>(const Vector2& v2) const { return (x > v2.x) && (y > v2.y); }
		inline float& operator[](unsigned int index) {
			if (index == 0) return x;
			if (index == 1) return y;
			throw std::runtime_error("Index out of range");
		}

		inline void Normalise() { (*this) *= 1.0f / sqrtf(Dot((*this))); }
		inline float Magnitude() const { return fabsf(x) + fabsf(y); }
		inline float Dot(const Vector2& v2) const { return x * v2.x + y * v2.y; };
		inline float Distance(const Vector2& v2) const { return sqrtf(powf(x - v2.x, 2) + powf(y - v2.y, 2)); };

		inline Vector2 Normalised() const {
			Vector2 copy = Vector2(x, y);
			copy.Normalise();
			return copy;
		}

		inline std::string ToString(PrintFormatArgs fmtArgs = PrintFormatArgs()) const {
			std::string xStr = fmtArgs.AsInt ? std::to_string((int)x) : std::to_string(x);
			std::string yStr = fmtArgs.AsInt ? std::to_string((int)y) : std::to_string(y);
			if (fmtArgs.Pretty) {
				return std::string("x: " + xStr + "\ny: " + yStr);
			} else {
				return std::string("{" + xStr + ", " + yStr + "}");
			}
		}
	};

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

		inline Vector3& operator=(const Vector3& v2) {
			if (this != &v2) { x = v2.x; y = v2.y; z = v2.z; }
			return *this;
		}

		inline Vector3 operator-() const { return Vector3(-x, -y, -z); }


		inline Vector3 operator+(Vector3 v2) const { return Vector3(x + v2.x, y + v2.y, z + v2.z); }
		inline Vector3 operator-(Vector3 v2) const { return Vector3(x - v2.x, y - v2.y, z - v2.z); }
		inline Vector3 operator*(Vector3 v2) const { return Vector3(x * v2.x, y * v2.y, z * v2.z); }
		inline Vector3 operator*(float n) const { return Vector3(x * n, y * n, z * n); }

		inline void operator+=(Vector3 v2) { x += v2.x; y += v2.y; z += v2.z; }
		inline void operator-=(Vector3 v2) { x -= v2.x; y -= v2.y; z -= v2.z; }
		inline void operator*=(Vector3 v2) { x *= v2.x; y *= v2.y; z *= v2.z; }
		inline void operator*=(float n) { x *= n; y *= n; z *= n; }

		bool operator==(const Vector3& v2) const = default;
		inline bool operator<(const Vector3& v2) const { return (x < v2.x) && (y < v2.y) && (z < v2.z); }
		inline bool operator>(const Vector3& v2) const { return (x > v2.x) && (y > v2.y) && (z > v2.z); }
		inline float& operator[](unsigned int index) {
			if (index == 0) return x;
			if (index == 1) return y;
			if (index == 2) return z;
			throw std::runtime_error("Index out of range");
		}

		inline Vector3 operator*(Vector2 v2) const { return Vector3(x * v2.x, y * v2.y, z); }
		inline Vector3 operator+(Vector2 v2) const { return Vector3(x + v2.x, y + v2.y, z); }
		inline Vector3 operator-(Vector2 v2) const { return Vector3(x - v2.x, y - v2.y, z); }

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

		inline std::string ToString(PrintFormatArgs fmtArgs = PrintFormatArgs()) const {
			std::string xStr = fmtArgs.AsInt ? std::to_string((int)x) : std::to_string(x);
			std::string yStr = fmtArgs.AsInt ? std::to_string((int)y) : std::to_string(y);
			std::string zStr = fmtArgs.AsInt ? std::to_string((int)z) : std::to_string(z);
			if (fmtArgs.Pretty) {
				return std::string("x: " + xStr + "\ny: " + yStr + "\nz: " + zStr);
			} else {
				return std::string("{" + xStr + ", " + yStr + ", " + zStr + "}");
			}
		}
	};

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

		inline Vector4& operator=(const Vector4& v2) {
			if (this != &v2) {
				x = v2.x;
				y = v2.y;
				z = v2.z;
				w = v2.w;
			}
			return *this;
		}

		inline Vector4 operator-() const {
			return Vector4(-x, -y, -z, -w);
		}


		inline Vector4 operator+(Vector4 v2) const { return Vector4(x + v2.x, y + v2.y, z + v2.z, w + v2.w); }
		inline Vector4 operator-(Vector4 v2) const { return Vector4(x - v2.x, y - v2.y, z - v2.z, w - v2.w); }
		inline Vector4 operator*(Vector4 v2) const { return Vector4(x * v2.x, y * v2.y, z * v2.z, w * v2.w); }
		inline Vector4 operator*(float n) const { return Vector4(x * n, y * n, z * n, w * n); }

		inline void operator+=(Vector4 v2) { x += v2.x; y += v2.y; z += v2.z; w += v2.w; }
		inline void operator-=(Vector4 v2) { x -= v2.x; y -= v2.y; z -= v2.z; w -= v2.w; }
		inline void operator*=(Vector4 v2) { x *= v2.x; y *= v2.y; z *= v2.z; w *= v2.w; }
		inline void operator*=(float n) { x *= n; y *= n; z *= n; w *= n; }

		bool operator==(const Vector4& v2) const = default;
		inline bool operator<(const Vector4& v2) const { return (x < v2.x) && (y < v2.y) && (z < v2.z) && (w < v2.w); }
		inline bool operator>(const Vector4& v2) const { return (x > v2.x) && (y > v2.y) && (z > v2.z) && (w > v2.w); }
		inline float& operator[](unsigned int index) {
			if (index == 0) return x;
			if (index == 1) return y;
			if (index == 2) return z;
			if (index == 3) return w;
			throw std::runtime_error("Index out of range");
		}

		// Compatibility with smaller vectors
		inline Vector4 operator*(Vector3 v2) const { return Vector4(x * v2.x, y * v2.y, z * v2.z, w); }
		inline Vector4 operator+(Vector3 v2) const { return Vector4(x + v2.x, y + v2.y, z + v2.z, w); }
		inline Vector4 operator-(Vector3 v2) const { return Vector4(x - v2.x, y - v2.y, z - v2.z, w); }
		inline Vector4 operator*(Vector2 v2) const { return Vector4(x * v2.x, y * v2.y, z, w); }
		inline Vector4 operator+(Vector2 v2) const { return Vector4(x + v2.x, y + v2.y, z, w); }
		inline Vector4 operator-(Vector2 v2) const { return Vector4(x - v2.x, y - v2.y, z, w); }

		inline void operator+=(Vector3 v2) {
			x += v2.x;
			y += v2.y;
			z += v2.z;
		}
		inline void operator-=(Vector3 v2) {
			x -= v2.x;
			y -= v2.y;
			z -= v2.z;
		}

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