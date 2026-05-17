#pragma once

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

#include "Transform.h"
#include <EngineLog.h>

class BaseCamera
{
public:
	static BaseCamera* ActiveCamera;

	Transform mTransform;
	float mFOVy = 70.0f;

	BaseCamera();

	void Move(glm::vec3 dirInput, glm::vec3 angInput);
	glm::mat4 GetViewMatrix() const { return glm::lookAt(mTransform.position, mTransform.position + mTransform.GetForwardVector(), mTransform.GetUpVector()); };

private:
	glm::vec3 mCameraTarget;
	float mYaw = 0;
	float mPitch = 0;
	float mCameraSpeed = 1;
	float mCameraSensitivity = 1;
};