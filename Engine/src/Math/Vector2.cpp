#include "Vector2.h"
#include <tgmath.h>

using Refraction::Math::Vector2;

float Vector2::Dot(Vector2 v1, Vector2 v2) {
	return (v1.x * v2.x) + (v1.y * v2.y);
}

float Vector2::Distance(const Vector2 v1, const Vector2 v2) {
	float xDiff = v1.x - v2.x;
	float yDiff = v1.y - v2.y;
	return sqrtf(powf(xDiff, 2) + powf(yDiff, 2));
}

void Vector2::Normalise() {
	auto mag = this->Magnitude();
	if (mag <= 0) return;
	x /= mag;
	y /= mag;
}
