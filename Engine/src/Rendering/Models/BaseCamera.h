#pragma once

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

#include "Transform.h"
#include "Log.h"

class BaseCamera
{
public:
	Transform m_Transform;
	float FOVy = 70.0f;

	BaseCamera();

	void Move(glm::vec3 dirInput, glm::vec3 angInput);
	void SetCameraSpeed(float newSpeed) { m_cameraSpeed = newSpeed; };
	void SetCameraSensitivity(float newSensitivity) { m_cameraSensitivity = newSensitivity; };
	glm::mat4 GetViewMatrix() const { return glm::lookAt(m_Transform.position, m_Transform.position + m_Transform.GetForwardVector(), m_Transform.GetUpVector()); };

private:
	friend class Window;

	glm::vec3 m_cameraTarget;

	float m_yaw;
	float m_pitch;

	float m_cameraSpeed;
	float m_cameraSensitivity;
};