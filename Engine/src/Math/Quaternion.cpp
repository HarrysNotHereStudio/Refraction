#include <Math/Matrices.h>
#include <Math/Vector2.h>

#include "Quaternion.h"

namespace RMath = Refraction::Math;
using RMath::Vector2;
using RMath::Vector3;

RMath::Quaternion RMath::Quaternion::FromAxisAngle(float a, const Vector3& vec) {
	Quaternion newQuat;

	auto halfAngle = RMath::ToRadians(a / 2);
	float sinHalfAngle = sinf(halfAngle);
	float cosHalfAngle = cosf(halfAngle);

	newQuat.x = vec.x * sinHalfAngle;
	newQuat.y = vec.y * sinHalfAngle;
	newQuat.z = vec.z * sinHalfAngle;
	newQuat.w = cosHalfAngle;
	return newQuat;
}

RMath::Quaternion RMath::Quaternion::FromEulerAngles(const Vector3& vec) {
	const float cX = cosf(vec.x / 2), sX = sinf(vec.x / 2);
	const float cY = cosf(vec.y / 2), sY = sinf(vec.y / 2);
	const float cZ = cosf(vec.z / 2), sZ = sinf(vec.z / 2);

	return Quaternion(
		sX * cY * cZ + sY * sZ * cX,
		sY * cX * cZ - sX * sZ * cY,
		sX * sY * cZ + sZ * cX * cY,
		cX * cY * cZ - sX * sY * sZ
	);
}

RMath::Quaternion RMath::Quaternion::FromMatrix3(Matrix3 mat) { return mat.ToQuaternion(); }
RMath::Quaternion RMath::Quaternion::FromMatrix4(Matrix4 mat) { return mat.ToQuaternion(); }

RMath::Quaternion RMath::Quaternion::RotationBetweenEulerAngles(Vector3 start, Vector3 end) {
	start.Normalise();
	end.Normalise();

	auto cosTheta = start.Dot(end);
	Vector3 rotAxis;

	// Case for vectors facing opposite directions
	if (cosTheta < -1 + 0.001f) {
		rotAxis = Vector3::Front().Cross(start);

		if ((rotAxis * rotAxis).Magnitude() < 0.01) {
			rotAxis = Vector3::Right().Cross(start);
		}

		rotAxis.Normalise();
		return Quaternion::FromAxisAngle(180.0f, rotAxis);
	}

	rotAxis = start.Cross(end);

	auto s = sqrtf((1 + cosTheta) * 2);
	auto invs = 1 / s;

	return Quaternion(
		rotAxis.x * invs,
		rotAxis.y * invs,
		rotAxis.z * invs,
		s * 0.5f
	);
}

RMath::Quaternion RMath::Quaternion::LookIn(const Vector3& direction, const Vector3& up) {
	Matrix3 result;

	result[2] = -direction.Normalised();
	auto right = up.Cross(result[2]);
	result[0] = right * (1 / sqrtf(std::max(0.00001f, right.Dot(right))));
	result[1] = result[2].Cross(result[0]);

	// Use Matrix4 constructor
	return Quaternion::FromMatrix3(result);
}

RMath::Quaternion RMath::Quaternion::LookAt(const Vector3& from, const Vector3& at, const Vector3& up) {
	return Matrix4::LookAt(from, at, up).ToQuaternion();
}

bool RMath::Quaternion::IsZero() const {
	return (!x) && (!y) && (!z) && (!w);
}

float RMath::Quaternion::Dot(const Quaternion& other) const {
	return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

Vector3 RMath::Quaternion::ToEulerAngles() const {
	Vector3 result;

	result.x = RMath::ToDegrees(atan2f((x * z) + (y * w), (x * w) - (y * z)));
	result.y = RMath::ToDegrees(acosf(-(x * x) - (y * y) - (z * z) - (w * w)));
	result.z = RMath::ToDegrees(atan2f((x * z) - (y * w), (x * w) + (y * z)));

	return result;
}

void RMath::Quaternion::Normalize() {
	if (IsZero()) return;
	const float nSq = x * x + y * y + z * z + w * w;
	const float inv = 1.0f / sqrtf(nSq);
	w *= inv;
	x *= inv;
	y *= inv;
	z *= inv;
}

RMath::Quaternion RMath::Quaternion::SLerp(Quaternion other, float time) const {
	float cosA = x * other.x + y * other.y + z * other.z + w * other.w;
	if (cosA < 0.0f) {
		cosA = -cosA;
		other = -other;
	}

	// use Lerp if close to end
	if (cosA > 0.999f) {
		return Quaternion(RMath::Lerp(x, other.x, time), RMath::Lerp(y, other.y, time), RMath::Lerp(z, other.z, time), RMath::Lerp(w, other.w, time));
	}

	float alpha = acosf(cosA);
	Quaternion result(*this);

	result *= sinf(1.0f - time);
	other *= sinf(time * alpha);

	return (result + other) / sinf(alpha);
}

RMath::Quaternion RMath::Quaternion::NLerp(Quaternion other, float time) const {
	const float l2 = Dot(other);
	if (l2 < 0.0f) {
		other = -other;
	}
	return Quaternion(RMath::Lerp(x, other.x, time), RMath::Lerp(y, other.y, time), RMath::Lerp(z, other.z, time), RMath::Lerp(w, other.w, time));
}

RMath::Vector3 Refraction::Math::operator*(const Quaternion& q, const Vector3& v) {
	Vector3 quatVec(q.x, q.y, q.z);
	Vector3 uv(quatVec.Cross(v));
	Vector3 uuv(quatVec.Cross(uv));

	return v + ((uv * q.w) + uuv) * 2.0f;
}

Vector3 Refraction::Math::operator*(Vector3 v, const Quaternion& q) {
	return q.Inverse() * v;
}
