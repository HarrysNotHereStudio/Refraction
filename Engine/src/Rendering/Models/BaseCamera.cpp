#include "../Settings.h"

#include "BaseCamera.h"


BaseCamera* BaseCamera::ActiveCamera = nullptr;

BaseCamera::BaseCamera() {
	mTransform = Transform();
	mTransform.Translate(glm::vec3(0.0f, 0.0f, -5.0f));
	mTransform.Rotate(glm::vec3(0.0f,glm::radians(180.0f),0.0f));
	mCameraTarget = mTransform.position + mTransform.GetForwardVector();
}

void BaseCamera::Move(glm::vec3 dirInput, glm::vec3 angInput) {
	// Normalize inputs
	if (dirInput != glm::vec3(0.0f)) dirInput = glm::normalize(dirInput);

	mCameraSensitivity = Settings::CurrentSettings->Controls.CameraSensitivity;
	mCameraSpeed = Settings::CurrentSettings->Controls.CameraSpeed;

	// Move camera
	glm::vec3 translateDelta = glm::vec3();
	if (abs(dirInput.z) > 0.0f) translateDelta += (dirInput.z * mCameraSpeed) * mTransform.GetForwardVector();
	if (abs(dirInput.y) > 0.0f) translateDelta += mTransform.GetUpVector() * (dirInput.y * mCameraSpeed);
	if (abs(dirInput.x) > 0.0f) translateDelta += mTransform.GetRightVector() * (dirInput.x * mCameraSpeed);

	// Rotate camera
	mYaw += angInput.y * mCameraSensitivity;
	mPitch = std::clamp(mPitch + angInput.x * mCameraSensitivity, -80.0f, 80.0f);

	glm::vec3 targetRotation = glm::vec3();
	targetRotation.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	targetRotation.y = sin(glm::radians(mPitch));
	targetRotation.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

	mTransform.Translate(translateDelta);
	mTransform.orientation = glm::quatLookAt(targetRotation, glm::vec3(0,1,0));

	mCameraTarget = mTransform.position + mTransform.GetForwardVector();
}