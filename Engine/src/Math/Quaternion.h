#pragma once

#include <string>

#include <Math/Common.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>

namespace Refraction::Math {
	class Matrix3;
	class Matrix4;

	class Quaternion {
	public:
		float x;
		float y;
		float z;
		float w;

		// Creates a Quaternion by directly setting the components
		Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
		// Creates a Quaternion using the components of a Vector4
		Quaternion(Vector4 vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w) {};
		// Creates an identity Quaternion
		Quaternion() : x(0), y(0), z(0), w(1) {};

		// Creates a Quaternion from an angle (degrees) and an axis
		static Quaternion FromAxisAngle(float a, const Vector3& vec);
		// Creates a Quaternion from a rotational Vector3 (degrees)
		static Quaternion FromEulerAngles(const Vector3& vec);
		// Creates a Quaternion from a Matrix3
		static Quaternion FromMatrix3(Matrix3 mat);
		// Creates a Quaternion from a Matrix4
		static Quaternion FromMatrix4(Matrix4 mat);
		// Creates a Quaternion using the angular difference between two vectors
		static Quaternion RotationBetweenEulerAngles(Vector3 start, Vector3 end);
		// Creates a Quaternion using a given direction
		static Quaternion LookIn(const Vector3& direction, const Vector3& up = Vector3::Up());
		// Creates a Quaternion using a given eye and target
		static Quaternion LookAt(const Vector3& from, const Vector3& at, const Vector3& up = Vector3::Up());

		static inline bool AreSimilar(const Quaternion& q1, const Quaternion& q2) {
			return (abs(q1.Dot(q2) - 1.0) < 0.001);
		}

		Quaternion(const Quaternion& other) {
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
		Quaternion& operator=(const Quaternion& other) {
			if (this != &other) {
				x = other.x;
				y = other.y;
				z = other.z;
				w = other.w;
			}
			return *this;
		}

		inline operator Vector4() const {
			return Vector4(x, y, z, w);
		}

		Quaternion operator-() const {
			return Quaternion(-x, -y, -z, -w);
		}
		Quaternion operator+(const Quaternion& other) const {
			return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
		}
		Quaternion operator-(const Quaternion& other) const {
			return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
		}
		Quaternion operator*(const Quaternion& other) const {
			Vector3 thisVec(x, y, z);
			Vector3 otherVec(other.x, other.y, other.z);

			Vector3 resultVec = otherVec * w + thisVec * other.w + thisVec.Cross(otherVec);
			float resultW = w * other.w - thisVec.Dot(otherVec);
			return Quaternion(resultVec.x, resultVec.y, resultVec.z, resultW);
		};
		Quaternion operator*(const float& n) const {
			return Quaternion(x * n, y * n, z * n, w * n);
		};
		Quaternion operator/(const float& n) const {
			return Quaternion(x / n, y / n, z / n, w / n);
		}

		void operator+=(const Quaternion& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
		}
		void operator-=(const Quaternion& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
		}

		void operator*=(const Quaternion& other) {
			Vector3 thisVec(x, y, z);
			Vector3 otherVec(other.x, other.y, other.z);

			Vector3 resultVec = otherVec * w + thisVec * other.w + thisVec.Cross(otherVec);
			x = resultVec.x;
			y = resultVec.y;
			z = resultVec.z;
			w = w * other.w - thisVec.Dot(otherVec);
		}
		void operator*=(const float& n) {
			x *= n;
			y *= n;
			z *= n;
			w *= n;
		}

		bool operator==(const Quaternion& v) const {
			return AreSimilar(*this, v);
		}

		void Normalize();

		Vector3 ToEulerAngles() const;
		bool IsZero() const;
		float Dot(const Quaternion& other) const;

		Quaternion SLerp(Quaternion other, float time) const;
		Quaternion NLerp(Quaternion other, float time) const;

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

		inline Quaternion Conjugate() const { return Quaternion(-x, -y, -z, w); }

		inline Quaternion Normalized() const {
			Quaternion copy = Quaternion(x, y, z, w);
			copy.Normalize();
			return copy;
		}

		inline Quaternion Inverse() const {
			if (IsZero()) return Quaternion(*this);
			return Conjugate() / Dot(*this);
		}

		inline void ToAxisAngle(float& angle, Vector3& axis) const {
			float cosHalfAngle = w;
			float halfAngle = acosf(cosHalfAngle);
			float sinHalfAngle = sinf(halfAngle);
			angle = halfAngle * 2;

			axis.x = x / sinHalfAngle;
			axis.y = y / sinHalfAngle;
			axis.z = z / sinHalfAngle;
		};

		inline void ResetNANs() {
			if (x != x) x = 0;
			if (y != y) y = 0;
			if (z != z) z = 0;
			if (w != w) w = 0;
		}
	};
	Vector3 operator*(const Quaternion& q, const Vector3& v);
	Vector3 operator*(Vector3 v, const Quaternion& q);
}
