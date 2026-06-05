#include "Vector3.h"
#include <tgmath.h>

using Refraction::Math::Vector3;

float Vector3::Dot(const Vector3& v2) const {
	return (x * v2.x) + (y * v2.y) + (z * v2.z);
}

Vector3 Vector3::Cross(const Vector3& v2) const {
	return Vector3((y * v2.z) - (z * v2.y), (z * v2.x) - (x * v2.z), (x * v2.y) - (y * v2.x));
}

float Vector3::Distance(const Vector3 v1, const Vector3 v2) {
	float xDiff = v1.x - v2.x;
	float yDiff = v1.y - v2.y;
	float zDiff = v1.z - v2.z;
	return sqrtf(powf(xDiff, 2) + powf(yDiff, 2) + powf(zDiff, 2));
}

void Vector3::Normalise() {
	auto mag = this->Magnitude();
	if (mag <= 0) return;
	x /= mag;
	y /= mag;
	z /= mag;
}
