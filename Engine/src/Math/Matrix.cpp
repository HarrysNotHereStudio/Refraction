#include "Matrix.h"

namespace Refraction::Math {
	Vector3 Matrix3::ToEulerAngles() {
		Vector3 newVec;
		float T1 = atan2f(m[2][1], m[2][2]);
		float C2 = sqrtf(m[0][0] * m[0][0] + m[1][0] * m[1][0]);
		float T2 = atan2f(-m[2][0], C2);
		float S1 = sinf(T1);
		float C1 = cosf(T1);
		float T3 = atan2f(S1 * m[0][2] - C1 * m[0][1], C1 * m[1][1] - S1 * m[1][2]);
		newVec.x = -T1;
		newVec.y = -T2;
		newVec.z = -T3;
		return newVec;
	}

	Quaternion Matrix3::ToQuaternion() {
		float x = m[0][0] - m[1][1] - m[2][2];
		float y = m[1][1] - m[0][0] - m[2][2];
		float z = m[2][2] - m[0][0] - m[1][1];
		float w = m[0][0] + m[1][1] + m[2][2];

		int biggestIndex = 0;
		float biggest = x;
		if (y > biggest) {
			biggest = y;
			biggestIndex = 1;
		}
		if (z > biggest) {
			biggest = z;
			biggestIndex = 2;
		}
		if (w > biggest) {
			biggest = w;
			biggestIndex = 3;
		}

		float biggestVal = sqrtf(biggest + 1.0f) * 0.5f;
		float mult = 0.25f / biggestVal;

		switch (biggestIndex) {
		case 0:
			return Quaternion(biggestVal, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] - m[2][1]) * mult);
		case 1:
			return Quaternion((m[0][1] + m[1][0]) * mult, biggestVal, (m[1][2] + m[2][1]) * mult, (m[2][0] - m[0][2]) * mult);
		case 2:
			return Quaternion((m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggestVal, (m[0][1] - m[1][0]) * mult);
		case 3:
			return Quaternion((m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult, biggestVal);
		default:
			return Quaternion();
		}
	}

	Matrix4 Matrix4::LookAt(const Vector3& from, const Vector3& at, const Vector3& up) {
		const Vector3 forward = (at - from).Normalised();
		const Vector3 right = forward.Cross(up).Normalised();
		const Vector3 matrixUp = right.Cross(forward);

		Matrix4 newMat;
		newMat[0][0] = right.x;
		newMat[1][0] = right.y;
		newMat[2][0] = right.z;
		newMat[0][1] = matrixUp.x;
		newMat[1][1] = matrixUp.y;
		newMat[2][1] = matrixUp.z;
		newMat[0][2] = -forward.x;
		newMat[1][2] = -forward.y;
		newMat[2][2] = -forward.z;
		newMat[3][0] = -right.Dot(from);
		newMat[3][1] = -matrixUp.Dot(from);
		newMat[3][2] = forward.Dot(from);

		return newMat;
	}

	Matrix4 Matrix4::Perspective(float fovY, float aspectRatio, float zNear, float zFar) {
		const float tanHalfFovy = tanf(fovY / 2.0f);

		Matrix4 newMat;
		newMat[0][0] = 1.0f / (aspectRatio * tanHalfFovy);
		newMat[1][1] = 1.0f / (tanHalfFovy);
		newMat[2][2] = -(zFar + zNear) / (zFar - zNear);
		newMat[2][3] = -1.0f;
		newMat[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
		return newMat;
	}

	Matrix4 Matrix4::Perspective(const Frustum& frustum) {
		return Matrix4::Perspective(frustum.fovY, frustum.AspectRatio(), frustum.zNear, frustum.zFar);
	}

	Matrix4 Matrix4::FromTranslation(const Vector3& translation) {
		Matrix4 newMat(1);
		newMat[0][3] = translation.x;
		newMat[1][3] = translation.y;
		newMat[2][3] = translation.z;
		return newMat;
	}

	Matrix4 Matrix4::FromRotation(const float& angle, Vector3 axis) {
		axis.Normalise();

		const float angSin = sinf(angle);
		const float angCos = cosf(angle);
		const float angCos1Min = 1.0f - angCos;

		const float xx = axis.x * axis.x;
		const float xy = axis.x * axis.y;
		const float xz = axis.x * axis.z;
		const float yy = axis.y * axis.y;
		const float yz = axis.y * axis.z;
		const float zz = axis.z * axis.z;

		return Matrix4(
			Vector4(angCos + xx * angCos1Min, xy * angCos1Min + axis.z * angSin, xz * angCos1Min - axis.y * angSin, 0),
			Vector4(xy * angCos1Min - axis.z * angSin, angCos * yy * angCos1Min, yz * angCos1Min + axis.x * angSin, 0),
			Vector4(xz * angCos1Min + axis.y * angSin, yz * angCos1Min - axis.x * angSin, angCos + zz * angCos1Min, 0),
			Vector4(0, 0, 0, 1)
		);
	}

	Matrix4 Matrix4::FromRotationX(const float& angle) {
		Matrix4 newMat(1);
		newMat[1][1] = cosf(angle);
		newMat[1][2] = sinf(angle);
		newMat[2][1] = -sinf(angle);
		newMat[2][2] = cosf(angle);
		return newMat;
	}
	Matrix4 Matrix4::FromRotationY(const float& angle) {
		Matrix4 newMat(1);
		newMat[0][0] = cosf(angle);
		newMat[0][1] = sinf(angle);
		newMat[1][0] = -sinf(angle);
		newMat[1][1] = cosf(angle);
		return newMat;
	}
	Matrix4 Matrix4::FromRotationZ(const float& angle) {
		Matrix4 newMat(1);
		newMat[1][1] = cosf(angle);
		newMat[1][2] = sinf(angle);
		newMat[2][1] = -sinf(angle);
		newMat[2][2] = cosf(angle);
		return newMat;
	}

	Matrix4 Matrix4::FromRotation(const Quaternion& rotation) {
		Matrix4 newMat(1);
		Vector3 rot = rotation.ToEulerAngles();
		Matrix4 rx = FromRotationX(rot.x);
		Matrix4 ry = FromRotationY(rot.y);
		Matrix4 rz = FromRotationZ(rot.z);

		return rz * ry * rx;
	}
	Matrix4 Matrix4::FromRotationZYX(const Quaternion& rotation) {
		Matrix4 newMat(1);
		Vector3 rot = rotation.ToEulerAngles();
		Matrix4 rx = FromRotationX(rot.x);
		Matrix4 ry = FromRotationY(rot.y);
		Matrix4 rz = FromRotationZ(rot.z);

		return rx * ry * rz;
	}

	Matrix4 Matrix4::FromScale(const Vector3& scale) {
		Matrix4 newMat(1);
		newMat[0][0] = scale.x;
		newMat[1][1] = scale.y;
		newMat[2][2] = scale.z;
		newMat[3][3] = 1.0f;
		return newMat;
	}

	Matrix4 Matrix4::Rotate(Quaternion quat) {
		float angle;
		Vector3 axis;
		quat.ToAxisAngle(angle, axis);
		const float cosA = cosf(angle);
		const float sinA = sinf(angle);

		Vector3 temp(axis * (1.0f - cosA));

		Matrix4 rotMat;
		rotMat[0][0] = cosA + temp.x * axis.x;
		rotMat[0][1] = temp.x * axis.y + sinA * axis.z;
		rotMat[0][2] = temp.x * axis.z - sinA * axis.y;

		rotMat[1][0] = temp.y * axis.x - sinA * axis.z;
		rotMat[1][1] = cosA + temp.y * axis.y;
		rotMat[1][2] = temp.y * axis.z + sinA * axis.x;

		rotMat[2][0] = temp.z * axis.x + sinA * axis.y;
		rotMat[2][1] = temp.z * axis.y - sinA * axis.x;
		rotMat[2][2] = cosA + temp.z * axis.z;

		Matrix4 result;
		result[0] = (*this)[0] * rotMat[0][0] + (*this)[1] * rotMat[0][1] + (*this)[2] * rotMat[0][2];
		result[1] = (*this)[0] * rotMat[1][0] + (*this)[1] * rotMat[1][1] + (*this)[2] * rotMat[1][2];
		result[2] = (*this)[0] * rotMat[2][0] + (*this)[1] * rotMat[2][1] + (*this)[2] * rotMat[2][2];
		result[3] = (*this)[3];
		return result;
	}

	Matrix4 Matrix4::Scale(Vector3 scale) {
		Matrix4 result(*this);
		result[0] *= scale.x;
		result[1] *= scale.y;
		result[2] *= scale.z;
		return result;
	}

	Vector3 Matrix4::ToEulerAngles() {
		Vector3 newVec;
		float T1 = atan2f(m[2][1], m[2][2]);
		float C2 = sqrtf(m[0][0] * m[0][0] + m[1][0] * m[1][0]);
		float T2 = atan2f(-m[2][0], C2);
		float S1 = sinf(T1);
		float C1 = cosf(T1);
		float T3 = atan2f(S1 * m[0][2] - C1 * m[0][1], C1 * m[1][1] - S1 * m[1][2]);
		newVec.x = -T1;
		newVec.y = -T2;
		newVec.z = -T3;
		return newVec;
	}

	Quaternion Matrix4::ToQuaternion() {
		float x = m[0][0] - m[1][1] - m[2][2];
		float y = m[1][1] - m[0][0] - m[2][2];
		float z = m[2][2] - m[0][0] - m[1][1];
		float w = m[0][0] + m[1][1] + m[2][2];

		int biggestIndex = 0;
		float biggest = x;
		if (y > biggest) {
			biggest = y;
			biggestIndex = 1;
		}
		if (z > biggest) {
			biggest = z;
			biggestIndex = 2;
		}
		if (w > biggest) {
			biggest = w;
			biggestIndex = 3;
		}

		float biggestVal = sqrtf(biggest + 1.0f) * 0.5f;
		float mult = 0.25f / biggestVal;

		switch (biggestIndex) {
		case 0:
			return Quaternion(biggestVal, (m[0][1] + m[1][0]) * mult, (m[2][0] + m[0][2]) * mult, (m[1][2] - m[2][1]) * mult);
		case 1:
			return Quaternion((m[0][1] + m[1][0]) * mult, biggestVal, (m[1][2] + m[2][1]) * mult, (m[2][0] - m[0][2]) * mult);
		case 2:
			return Quaternion((m[2][0] + m[0][2]) * mult, (m[1][2] + m[2][1]) * mult, biggestVal, (m[0][1] - m[1][0]) * mult);
		case 3:
			return Quaternion((m[1][2] - m[2][1]) * mult, (m[2][0] - m[0][2]) * mult, (m[0][1] - m[1][0]) * mult, biggestVal);
		default:
			return Quaternion();
		}
	}

	Matrix4 Matrix4::FromRotation(const Orientation& rotation) {
		auto vec = rotation.ToVector3();
		Matrix4 rx = FromRotationX(ToRadians(vec.x));
		Matrix4 ry = FromRotationY(ToRadians(vec.y));
		Matrix4 rz = FromRotationZ(ToRadians(vec.z));

		return rz * ry * rx;
	}

	Matrix4 Matrix4::FromRotationZYX(const Orientation& rotation) {
		auto vec = rotation.ToVector3();
		Matrix4 rx = FromRotationX(ToRadians(vec.x));
		Matrix4 ry = FromRotationY(ToRadians(vec.y));
		Matrix4 rz = FromRotationZ(ToRadians(vec.z));

		return rx * ry * rz;
	}

	Matrix4 Matrix4::Rotate(Orientation orientation) {
		return (*this) * Matrix4::FromRotation(orientation);
	}

	Orientation Matrix4::ToOrientation() {
		float T1 = atan2f(m[2][1], m[2][2]);
		float C2 = sqrtf(m[0][0] * m[0][0] + m[1][0] * m[1][0]);
		float T2 = atan2f(-m[2][0], C2);
		float S1 = sinf(T1);
		float C1 = cosf(T1);
		float T3 = atan2f(S1 * m[0][2] - C1 * m[0][1], C1 * m[1][1] - S1 * m[1][2]);

		return Orientation(Vector3(-T1, -T2, -T3));
	}

	Vector3 Matrix4::GetTranslation() { return Vector3(m[0][3], m[1][3], m[2][3]); }

	Vector3 Matrix4::GetScale() { return Vector3(m[0][0], m[1][1], m[2][2]); }

	Matrix4 Matrix4::Inverse() {
		float c0 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		float c2 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		float c3 = m[1][2] * m[2][3] - m[2][2] * m[1][3];

		float c4 = m[2][1] * m[3][3] - m[3][1] * m[2][3];
		float c6 = m[1][1] * m[3][3] - m[3][1] * m[1][3];
		float c7 = m[1][1] * m[2][3] - m[2][1] * m[1][3];

		float c8 = m[2][1] * m[3][2] - m[3][1] * m[2][2];
		float c10 = m[1][1] * m[3][2] - m[3][1] * m[1][2];
		float c11 = m[1][1] * m[2][2] - m[2][1] * m[1][2];

		float c12 = m[2][0] * m[3][3] - m[3][0] * m[2][3];
		float c14 = m[1][0] * m[3][3] - m[3][0] * m[1][3];
		float c15 = m[1][0] * m[2][3] - m[2][0] * m[1][3];

		float c16 = m[2][0] * m[3][2] - m[3][0] * m[2][2];
		float c18 = m[1][0] * m[3][2] - m[3][0] * m[1][2];
		float c19 = m[1][0] * m[2][2] - m[2][0] * m[1][2];

		float c20 = m[2][0] * m[3][1] - m[3][0] * m[2][1];
		float c22 = m[1][0] * m[3][1] - m[3][0] * m[1][1];
		float c23 = m[1][0] * m[2][1] - m[2][0] * m[1][1];

		Vector4 f0(c0, c0, c2, c3);
		Vector4 f1(c4, c4, c6, c7);
		Vector4 f2(c8, c8, c10, c11);
		Vector4 f3(c12, c12, c14, c15);
		Vector4 f4(c16, c16, c18, c19);
		Vector4 f5(c20, c20, c22, c23);

		Vector4 v0(m[1][0], m[0][0], m[0][0], m[0][0]);
		Vector4 v1(m[1][1], m[0][1], m[0][1], m[0][1]);
		Vector4 v2(m[1][2], m[0][2], m[0][2], m[0][2]);
		Vector4 v3(m[1][3], m[0][3], m[0][3], m[0][3]);

		Vector4 inv0(v1 * f0 - v2 * f1 + v3 * f2);
		Vector4 inv1(v0 * f0 - v2 * f3 + v3 * f4);
		Vector4 inv2(v0 * f1 - v1 * f3 + v3 * f5);
		Vector4 inv3(v0 * f1 - v1 * f4 + v2 * f5);

		Vector4 signA(+1, -1, +1, -1);
		Vector4 signB(-1, +1, -1, +1);
		Matrix4 invMat(inv0 * signA, inv1 * signB, inv2 * signA, inv3 * signB);

		Vector4 r0(invMat[0][0], invMat[1][0], invMat[2][0], invMat[3][0]);

		Vector4 d0((*this)[0]);
		d0 *= r0;
		float d1 = (d0.x + d0.y) + (d0.z + d0.w);

		float OneOverDeterminant = 1.0f / d1;

		return invMat * OneOverDeterminant;
	}

}
