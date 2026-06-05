#include "Transform.h"

namespace RMath = Refraction::Math;
namespace RUtilities = Refraction::Utilities;

using RMath::Matrix4;
using RMath::Vector3;
using RMath::Quaternion;

RMath::Transform::Transform() {
	mPosition = Vector3(0.0f);
	mOrientation = Quaternion();
	mScale = Vector3(1.0f);
}

void RMath::Transform::Translate(Vector3 delta) {
	mPosition += delta;
}

void RMath::Transform::Rotate(float angle, Vector3 axis) {
	auto rotQ = Quaternion::FromAxisAngle(angle, axis);
	auto conjQ = rotQ.Conjugate();
	mOrientation = rotQ * mOrientation * conjQ;
}

void RMath::Transform::Rotate(Vector3 delta) {
	mOrientation = (mOrientation * Quaternion::FromEulerAngles(delta)).Normalized();
}

void RMath::Transform::Rotate(Quaternion delta) {
	mOrientation *= delta;
}

void RMath::Transform::Scale(Vector3 delta) {
	mScale += delta;
}

Matrix4 RMath::Transform::GetTransform() const {
	Matrix4 transform = Matrix4();
	transform = transform.Translate(mPosition);
	transform = transform.Rotate(RMath::ToRadians(mOrientation.x), Vector3::Right());
	transform = transform.Rotate(RMath::ToRadians(mOrientation.y), Vector3::Up());
	transform = transform.Rotate(RMath::ToRadians(mOrientation.z), Vector3::Front());
	transform = transform.Scale(mScale);
	return transform;
}
