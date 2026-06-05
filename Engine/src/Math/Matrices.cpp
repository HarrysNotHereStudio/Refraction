#include "Matrices.h"

namespace RMath = Refraction::Math;


inline RMath::Matrix3 RMath::Matrix3::operator*(Matrix3 other) const {
	Matrix3 result(*this);
	result *= other;
	return result;
}

inline void RMath::Matrix3::operator*=(Matrix3 other) {
	if (other.mSize != mSize) throw std::runtime_error("Matrix sizes do not match");
	Matrix3 result;
	result.ResizeMatrix(mSize);

	for (unsigned int i = 0; i < mSize; i++) {
		for (unsigned int j = 0; j < mSize; j++) {
			result[i][j] = 0;

			for (unsigned int k = 0; k < mSize; k++) {
				result[i][j] += m[i][k] * other[k][j];
			}
		}
	}
	m = result.m;
}


RMath::Matrix4::Matrix4(const Vector3& from, const Vector3& at, const Vector3& up) {
	const Vector3 forward = (at - from).Normalised();
	const Vector3 right = forward.Cross(up).Normalised();
	const Vector3 matrixUp = right.Cross(forward);

	m[0][0] = right.x;
	m[1][0] = right.y;
	m[2][0] = right.z;
	m[0][1] = matrixUp.x;
	m[1][1] = matrixUp.y;
	m[2][1] = matrixUp.z;
	m[0][2] = -forward.x;
	m[1][2] = -forward.y;
	m[2][2] = -forward.z;
	m[3][0] = -right.Dot(from);
	m[3][1] = -matrixUp.Dot(from);
	m[3][2] = forward.Dot(from);
}

RMath::Matrix4 RMath::Matrix4::Perspective(float fovY, float aspectRatio, float zNear, float zFar) {
	const float tanHalfFovy = tanf(fovY / 2.0f);

	Matrix4 newMat;
	newMat[0][0] = 1.0f / (aspectRatio * tanHalfFovy);
	newMat[1][1] = 1.0f / (tanHalfFovy);
	newMat[2][2] = -(zFar + zNear) / (zFar - zNear);
	newMat[2][3] = -1.0f;
	newMat[3][2] = -(2.0f * zFar * zNear) / (zFar - zNear);
	return newMat;
}

inline RMath::Matrix4 RMath::Matrix4::operator*(Matrix4 other) const {
	Matrix4 result(*this);
	result *= other;
	return result;
}

inline void RMath::Matrix4::operator*=(Matrix4 other) {
	if (other.mSize != mSize) throw std::runtime_error("Matrix sizes do not match");
	Matrix4 result;
	result.ResizeMatrix(mSize);

	for (unsigned int i = 0; i < mSize; i++) {
		for (unsigned int j = 0; j < mSize; j++) {
			result[i][j] = 0;

			for (unsigned int k = 0; k < mSize; k++) {
				result[i][j] += m[i][k] * other[k][j];
			}
		}
	}
	m = result.m;
}

RMath::Matrix4 RMath::Matrix4::Rotate(const float& angle, Vector3 axis) {
	const float cosA = cosf(angle);
	const float sinA = sinf(angle);

	axis.Normalise();
	Vector3 temp(axis * (1.0f - cosA));

	Matrix4 rotMatrix;
	rotMatrix[0][0] = cosA + temp.x * axis.x;
	rotMatrix[0][1] = temp.x * axis.y + sinA * axis.z;
	rotMatrix[0][2] = temp.x * axis.z - sinA * axis.y;

	rotMatrix[1][0] = temp.y * axis.x - sinA * axis.z;
	rotMatrix[1][1] = cosA + temp.y * axis.y;
	rotMatrix[1][2] = temp.y * axis.z + sinA * axis.x;

	rotMatrix[2][0] = temp.z * axis.x + sinA * axis.y;
	rotMatrix[2][1] = temp.z * axis.y - sinA * axis.x;
	rotMatrix[2][2] = cosA + temp.z * axis.z;

	Matrix4 newMat;
	Vector4 row0 = (*this)[0];
	Vector4 row1 = (*this)[1];
	Vector4 row2 = (*this)[2];
	newMat[0] = row0 * rotMatrix[0][0] + row1 * rotMatrix[0][1] + row2 * rotMatrix[0][2];
	newMat[1] = row0 * rotMatrix[1][0] + row1 * rotMatrix[1][1] + row2 * rotMatrix[1][2];
	newMat[2] = row0 * rotMatrix[2][0] + row1 * rotMatrix[2][1] + row2 * rotMatrix[2][2];
	newMat[3] = (*this)[3];
	return newMat;
}

RMath::Matrix4 RMath::Matrix4::Scale(Vector3 scale) {
	Matrix4 newMat;
	newMat[0] = (Vector4)(*this)[0] * scale.x;
	newMat[1] = (Vector4)(*this)[1] * scale.y;
	newMat[2] = (Vector4)(*this)[2] * scale.z;
	newMat[3] = (*this)[3];
	return newMat;
}
