#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Transform.h"

namespace RMath = Refraction::Math;
namespace RUtil = Refraction::Utilities;

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
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform,RUtil::NativeToGLMVec3(mPosition));
	transform = glm::rotate(transform, glm::radians(mOrientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(mOrientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(mOrientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, RUtil::NativeToGLMVec3(mScale));
	//Matrix4 transform = Matrix4();
	//transform = transform.Translate(mPosition);
	//transform = transform.Rotate(mOrientation);
	//transform = transform.Scale(mScale);
	return RUtil::GLMToNativeMat4(transform);
}
