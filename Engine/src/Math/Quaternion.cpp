#include <Math/Matrices.h>

#include "Quaternion.h"

namespace RMath = Refraction::Math;
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
	return Quaternion::FromAxisAngle(0, vec);
}

RMath::Quaternion RMath::Quaternion::FromMatrix3(Matrix3 mat) {
	float x = mat[0][0] - mat[1][1] - mat[2][2];
	float y = mat[1][1] - mat[0][0] - mat[2][2];
	float z = mat[2][2] - mat[0][0] - mat[1][1];
	float w = mat[0][0] + mat[1][1] + mat[2][2];

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
		return Quaternion(biggestVal, (mat[0][1] + mat[1][0]) * mult, (mat[2][0] + mat[0][2]) * mult, (mat[1][2] - mat[2][1]) * mult);
	case 1:
		return Quaternion((mat[0][1] + mat[1][0]) * mult, biggestVal, (mat[1][2] + mat[2][1]) * mult, (mat[2][0] - mat[0][2]) * mult);
	case 2:
		return Quaternion((mat[2][0] + mat[0][2]) * mult, (mat[1][2] + mat[2][1]) * mult, biggestVal, (mat[0][1] - mat[1][0]) * mult);
	case 3:
		return Quaternion((mat[1][2] - mat[2][1]) * mult, (mat[2][0] - mat[0][2]) * mult, (mat[0][1] - mat[1][0]) * mult, biggestVal);
	default:
		return Quaternion(0, 0, 0, 1);
	}
}

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
	auto result = Matrix3();

	result[2] = -direction;
	auto right = up.Cross(result[2]);
	result[0] = right * (1 / sqrtf(std::max(0.00001f, right.Dot(right))));
	result[1] = result[2].Cross(result[0]);

	// Use Matrix3 constructor
	return Quaternion::FromMatrix3(result);
}

RMath::Quaternion RMath::Quaternion::LookAt(const Vector3& from, const Vector3& at, const Vector3& up) {
	auto direction = at - from;

	auto rot1 = Quaternion::RotationBetweenEulerAngles(Vector3::Front(), direction);

	auto right = direction.Cross(up);
	auto targetUp = right.Cross(direction);

	auto newUp = Vector3::Up() * rot1;
	auto rot2 = Quaternion::RotationBetweenEulerAngles(newUp, targetUp);

	return rot2 * rot1;
}



bool RMath::Quaternion::IsZero() const {
	return (!x) && (!y) && (!z) && (!w);
}

float RMath::Quaternion::Dot(const Quaternion& other) const {
	return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

Vector3 RMath::Quaternion::ToDegrees() const {
	Vector3 vec(0);

	vec.x = RMath::ToDegrees(atan2f((x * z) + (y * w), (x * w) - (y * z)));
	vec.y = RMath::ToDegrees(acosf(-(x * x) - (y * y) - (z * z) - (w * w)));
	vec.z = RMath::ToDegrees(atan2f((x * z) - (y * w), (x * w) + (y * z)));

	return vec;
}

void RMath::Quaternion::Normalize() {
	float len = sqrtf((x * x) + (y * y) + (z * z) + (w * w));
	x /= len;
	y /= len;
	z /= len;
	w /= len;
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

RMath::Quaternion Refraction::Math::operator*(const Quaternion& l, const Quaternion& r) {
	auto quat = Quaternion();

	quat.w = ((l.w * r.w) - (l.x * r.x) - (l.y * r.y) - (l.z * r.z));
	quat.x = ((l.x * r.w) + (l.w * r.x) + (l.y * r.z) - (l.z * r.y));
	quat.y = ((l.y * r.w) + (l.w * r.y) + (l.z * r.x) - (l.x * r.z));
	quat.z = ((l.z * r.w) + (l.w * r.z) + (l.x * r.y) - (l.y * r.x));

	return quat;
}

RMath::Quaternion Refraction::Math::operator*(const Quaternion& q, const Vector3& v) {
	auto quat = Quaternion();

	quat.w = -((q.x * v.x) - (q.y * v.y) - (q.z * v.z));
	quat.x = ((q.w * v.x) + (q.y * v.z) - (q.z * v.y));
	quat.y = ((q.w * v.y) + (q.z * v.x) - (q.x * v.z));
	quat.z = ((q.w * v.z) + (q.x * v.y) - (q.y * v.x));

	return quat;
}

Vector3 Refraction::Math::operator*(Vector3 v, const Quaternion& q) {
	Vector3 quatVec(q.x, q.y, q.z);
	Vector3 uv(quatVec.Cross(v));
	Vector3 uuv(quatVec.Cross(uv));

	return v + ((uv * q.w) + uuv) * 2.0f;
}
