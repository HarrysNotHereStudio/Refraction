#include "Transform.h"

using glm::vec3, glm::mat4;

Transform::Transform() {
	position = vec3(0.0f);
	orientation = glm::quat(vec3(0.0f));
	scale = vec3(1.0f);
}

void Transform::Translate(vec3 delta) {
	position += delta;
}

void Transform::Rotate(vec3 delta) {
	orientation *= glm::quat(delta);
}

void Transform::Scale(vec3 delta) {
	scale += delta;
}

mat4 Transform::GetTransform() const {
	mat4 translationMatrix = glm::translate(mat4(1.0f), position);
	mat4 rotationMatrix = glm::toMat4(orientation);
	mat4 scaleMatrix = glm::scale(mat4(1.0f), scale);
	return translationMatrix * rotationMatrix * scaleMatrix;
}
