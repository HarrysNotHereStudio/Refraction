#pragma once

#include <string>

#include <Math/Common.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Matrices.h>

namespace Refraction::Math {
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
		Quaternion() : x(0), y(0), z(0), w(0) {};

		// Creates a Quaternion from an angle (degrees) and an axis
		static Quaternion FromAxisAngle(float a, const Vector3& vec);
		// Creates a Quaternion from a rotational Vector3
		static Quaternion FromEulerAngles(const Vector3& vec);
		// Creates a Quaternion from a Matrix3
		static Quaternion FromMatrix3(Matrix3 mat);
		// Creates a Quaternion using the angular difference between two vectors
		static Quaternion RotationBetweenEulerAngles(Vector3 start, Vector3 end);
		// Creates a Quaternion using a given direction
		static Quaternion LookIn(const Vector3& direction, const Vector3& up);
		// Creates a Quaternion using a given eye and target
		static Quaternion LookAt(const Vector3& from, const Vector3& at, const Vector3& up);

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

		inline operator Vector4() {
			return Vector4(x, y, z, w);
		}

		Quaternion operator-() {
			return Quaternion(-x, -y, -z, -w);
		}
		Quaternion operator+(const Quaternion& other) {
			return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
		}
		Quaternion operator-(const Quaternion& other) {
			return Quaternion(x - other.x, y - other.y, z - other.z, w - other.w);
		}
		Quaternion operator*(const float& n) {
			return Quaternion(x * n, y * n, z * n, w * n);
		}
		Quaternion operator/(const float& n) {
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
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
		}
		void operator*=(const float& n) {
			x *= n;
			y *= n;
			z *= n;
			w *= n;
		}

		void Normalize();

		Vector3 ToEulerAngles() const;
		bool IsZero() const;
		float Dot(const Quaternion& other) const;

		Quaternion SLerp(Quaternion other, float time) const;
		Quaternion NLerp(Quaternion other, float time) const;

		inline std::string ToString(bool pretty = true) const;

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
	};

	Quaternion operator*(const Quaternion& l, const Quaternion& r);
	Vector3 operator*(const Quaternion& q, const Vector3& v);
	Vector3 operator*(Vector3 v, const Quaternion& q);
}
