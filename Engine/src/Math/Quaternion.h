#pragma once

#include <Math/Common.h>
#include <Math/Vector3.h>
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
		// Creates an identity Quaternion
		Quaternion() : x(0), y(0), z(0), w(0) {};

		static Quaternion FromAxisAngle(float a, const Vector3& vec);
		static Quaternion FromEulerAngles(const Vector3& vec);
		static Quaternion FromMatrix3(Matrix3 mat);
		static Quaternion RotationBetweenEulerAngles(Vector3 start, Vector3 end);
		static Quaternion LookIn(const Vector3& direction, const Vector3& up);
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

		Vector3 ToDegrees() const;
		bool IsZero() const;
		float Dot(const Quaternion& other) const;

		void Normalize();
		Quaternion SLerp(Quaternion other, float time) const;
		Quaternion NLerp(Quaternion other, float time) const;

		inline Quaternion Conjugate() const { return Quaternion(-x, -y, -z, w); }
		inline Quaternion Normalized() const {
			Quaternion copy = Quaternion(x, y, z, w);
			copy.Normalize();
			return copy;
		}
	};

	Quaternion operator*(const Quaternion& l, const Quaternion& r);
	Quaternion operator*(const Quaternion& q, const Vector3& v);
	Vector3 operator*(Vector3 v, const Quaternion& q);
}
