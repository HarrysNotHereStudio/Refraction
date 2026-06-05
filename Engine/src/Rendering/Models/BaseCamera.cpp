#include <Core/Utilities.h>
#include <Settings.h>

#include "BaseCamera.h"

namespace RMath = Refraction::Math;

BaseCamera* BaseCamera::ActiveCamera = nullptr;

BaseCamera::BaseCamera() {
	mTransform = RMath::Transform();
	mTransform.Translate(RMath::Vector3::Front()*-5.0f);
	mTransform.Rotate(RMath::Vector3(0.0f,RMath::ToRadians(180.0f),0.0f));
	mCameraTarget = mTransform.mPosition + mTransform.GetForwardVector();
}

void BaseCamera::Move(RMath::Vector3 dirInput, RMath::Vector3 angInput) {
	// Normalize inputs
	if (dirInput.Magnitude() > 0) dirInput.Normalise();

	mCameraSensitivity = Settings::CurrentSettings->Controls.CameraSensitivity;
	mCameraSpeed = Settings::CurrentSettings->Controls.CameraSpeed;

	// Move camera
	RMath::Vector3 translateDelta = RMath::Vector3();
	if (abs(dirInput.z) > 0.0f) translateDelta += mTransform.GetForwardVector() * (dirInput.z * mCameraSpeed);
	if (abs(dirInput.y) > 0.0f) translateDelta += mTransform.GetUpVector() * (dirInput.y * mCameraSpeed);
	if (abs(dirInput.x) > 0.0f) translateDelta += mTransform.GetRightVector() * (dirInput.x * mCameraSpeed);

	// Rotate camera
	mYaw += angInput.y * mCameraSensitivity;
	mPitch = std::clamp(mPitch + angInput.x * mCameraSensitivity, -80.0f, 80.0f);

	RMath::Vector3 targetRotation = RMath::Vector3();
	targetRotation.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	targetRotation.y = sin(glm::radians(mPitch));
	targetRotation.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

	mTransform.Translate(translateDelta);
	mTransform.mOrientation = RMath::Quaternion::LookIn(targetRotation, RMath::Vector3::Up());

	mCameraTarget = mTransform.mPosition + mTransform.GetForwardVector();
}