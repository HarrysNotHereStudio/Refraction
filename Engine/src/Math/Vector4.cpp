#include <tgmath.h>

#include <Math/Vector3.h>

#include "Vector4.h"

using Refraction::Math::Vector4;


Vector4 Refraction::Math::Vector4::operator*(Vector3 v2) const {
	return Vector4(x * v2.x, y * v2.y, z * v2.z, w);
}
Vector4 Refraction::Math::Vector4::operator+(Vector3 v2) const {
	return Vector4(x + v2.x, y + v2.y, z + v2.z, w);
}
Vector4 Refraction::Math::Vector4::operator-(Vector3 v2) const {
	return Vector4(x - v2.x, y - v2.y, z - v2.z, w);
}

void Refraction::Math::Vector4::operator+=(Vector3 v2) {
	x += v2.x;
	y += v2.y;
	z += v2.z;
}
void Refraction::Math::Vector4::operator-=(Vector3 v2) {
	x -= v2.x;
	y -= v2.y;
	z -= v2.z;
}


float Vector4::Dot(const Vector4& v2) const {
	return (x * v2.x) + (y * v2.y) + (z * v2.z) + (w * v2.w);
}

float Vector4::Distance(const Vector4 v1, const Vector4 v2) {
	float xDiff = v1.x - v2.x;
	float yDiff = v1.y - v2.y;
	float zDiff = v1.z - v2.z;
	float wDiff = v1.z - v2.w;
	return sqrtf(powf(xDiff, 2) + powf(yDiff, 2) + powf(zDiff, 2) + powf(wDiff, 2));
}

void Vector4::Normalise() {
	auto mag = this->Magnitude();
	if (mag <= 0) return;
	x /= mag;
	y /= mag;
	z /= mag;
	w /= mag;
}
