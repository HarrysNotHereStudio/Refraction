#pragma once

#include <vector>
#include <stdexcept>
#include <string>

#include <Math/Common.h>
#include <Math/Vector.h>
#include <Math/Quaternion.h>
#include <Math/Orientation.h>
#include <Math/Frustum.h>

namespace Refraction::Math {
	class Matrix3 {
		static const uint8_t Size = 3;
	public:
		Vector3 m[Size] = {};

		Matrix3() {
			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					if (i == j) m[i][j] = 1;
				}
			}
		}
		Matrix3(float init) {
			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					if (i == j) m[i][j] = init;
				}
			}
		}

		Matrix3& operator=(const Matrix3& other) {
			if (this != &other) {
				for (uint8_t i = 0; i < Size; i++) {
					m[i] = other.m[i];
				}
			}
			return *this;
		}

		inline Vector3& operator[](uint8_t i) {
			return m[i];
		}

		inline Matrix3 operator*(Matrix3 other) {
			Matrix3 result;

			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					result[i][j] = 0;

					for (uint8_t k = 0; k < Size; k++) {
						result[i][j] += m[i][k] * other[k][j];
					}
				}
			}
			return result;
		}
		inline void operator*=(Matrix3 other) {
			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					for (uint8_t k = 0; k < Size; k++) {
						m[i][j] += m[i][k] * other[k][j];
					}
				}
			}
		}

		inline Matrix3 Transpose() {
			Matrix3 result;

			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					result[i][j] = m[j][i];
				}
			}
			return result;
		}

		// Convert the matrix to a rotational Vector3 (degrees)
		Vector3 ToEulerAngles();
		Quaternion ToQuaternion();

		inline std::string ToString(Refraction::Math::PrintFormatArgs fmtArgs = Refraction::Math::PrintFormatArgs()) {
			std::string out;
			for (uint8_t row = 0; row < Size; row++) {
				if (!fmtArgs.Pretty && row > 0) out += ", ";
				out += "{ ";
				for (uint8_t col = 0; col < Size; col++) {
					if (col > 0) out += ", ";
					out += fmtArgs.AsInt ? std::to_string((int)(m[row][col])) : std::to_string(m[row][col]);
				}
				out += (fmtArgs.Pretty) ? " }\n" : " }";
			}
			return out;
		}
	};

	class Matrix4 {
		static const uint8_t Size = 4;
	public:
		Vector4 m[Size] = {};

		Matrix4() {
			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					if (i == j) m[i][j] = 1;
				}
			}
		}
		Matrix4(float init) {
			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					if (i == j) m[i][j] = init;
				}
			}
		}
		Matrix4(Vector4 r0, Vector4 r1, Vector4 r2, Vector4 r3) {
			m[0] = r0;
			m[1] = r1;
			m[2] = r2;
			m[3] = r3;
		}

		static Matrix4 LookAt(const Vector3& from, const Vector3& at, const Vector3& up = Vector3::Up());
		static Matrix4 Perspective(float fovY, float aspectRatio, float zNear, float zFar);
		static Matrix4 Perspective(const Frustum& frustum);
		static Matrix4 FromTranslation(const Vector3& translation);
		static Matrix4 FromRotation(const float& angle, Vector3 axis);
		static Matrix4 FromRotationX(const float& angle);
		static Matrix4 FromRotationY(const float& angle);
		static Matrix4 FromRotationZ(const float& angle);
		static Matrix4 FromRotation(const Orientation& rotation);
		static Matrix4 FromRotation(const Quaternion& rotation);
		static Matrix4 FromRotationZYX(const Orientation& rotation);
		static Matrix4 FromRotationZYX(const Quaternion& rotation);
		static Matrix4 FromScale(const Vector3& scale);

		Matrix4& operator=(const Matrix4& other) {
			if (this != &other) {
				for (uint8_t i = 0; i < Size; i++) {
					m[i] = other.m[i];
				}
			}
			return *this;
		}

		inline Vector4& operator[](uint8_t i) {
			return m[i];
		}

		inline Matrix4 operator*(Matrix4 other) {
			Matrix4 result;

			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					result[i][j] = 0;

					for (uint8_t k = 0; k < Size; k++) {
						result[i][j] += m[i][k] * other[k][j];
					}
				}
			}
			return result;
		}
		inline void operator*=(Matrix4 other) {
			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					for (uint8_t k = 0; k < Size; k++) {
						m[i][j] += m[i][k] * other[k][j];
					}
				}
			}
		}

		Matrix4 Inverse();
		inline Matrix4 Transpose() {
			Matrix4 result;

			for (uint8_t i = 0; i < Size; i++) {
				for (uint8_t j = 0; j < Size; j++) {
					result[i][j] = m[j][i];
				}
			}
			return result;
		}

		// Translate matrix using a Vector3
		inline Matrix4 Translate(Vector3 v) {
			Matrix4 result(*this);
			//result[3] = (*this)[0] * v.x + (*this)[1] * v.y + (*this)[2] * v.z + (*this)[3];
			result[0][3] = m[0][3] * v.x;
			result[1][3] = m[1][3] * v.y;
			result[2][3] = m[2][3] * v.z;

			// nothing EVER FUCKING WORKS
			return result;
		}

		// Rotate using a Quaternion
		Matrix4 Rotate(Quaternion quat);
		Matrix4 Rotate(Orientation orientation);
		Matrix4 Scale(Vector3 scale);

		Vector3 GetTranslation();
		Vector3 GetScale();


		// Convert the matrix to a rotational Vector3 (degrees)
		Vector3 ToEulerAngles();
		Orientation ToOrientation();
		Quaternion ToQuaternion();

		inline std::string ToString(Refraction::Math::PrintFormatArgs fmtArgs = Refraction::Math::PrintFormatArgs()) {
			std::string out;
			for (uint8_t row = 0; row < Size; row++) {
				if (!fmtArgs.Pretty && row > 0) out += ", ";
				out += "{ ";
				for (uint8_t col = 0; col < Size; col++) {
					if (col > 0) out += ", ";
					out += fmtArgs.AsInt ? std::to_string((int)(m[row][col])) : std::to_string(m[row][col]);
				}
				out += (fmtArgs.Pretty) ? " }\n" : " }";
			}
			return out;
		}
	};
}
