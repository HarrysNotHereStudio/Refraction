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
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	transform = glm::rotate(transform, glm::radians(orientation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(orientation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(orientation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, scale);
	return transform;
}
