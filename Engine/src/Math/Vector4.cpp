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
