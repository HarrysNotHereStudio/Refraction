#include <Core/Utilities.h>
#include <Core/Log.h>
#include <Settings.h>

#include "BaseCamera.h"

namespace RMath = Refraction::Math;

BaseCamera* BaseCamera::ActiveCamera = nullptr;

BaseCamera::BaseCamera() {
	mTransform = RMath::Transform();
	mTransform.Translate(RMath::Vector3::Front()*-5.0f);
	mTransform.mOrientation = RMath::Quaternion::LookAt(mTransform.GetWorldPosition(), RMath::Vector3::Zero(), RMath::Vector3::Up());
	mCameraTarget = mTransform.GetWorldPosition() + mTransform.GetForwardVector();

	auto eulerRot = mTransform.mOrientation.ToEulerAngles();
	mPitch = eulerRot.x;
	mYaw = eulerRot.y;
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
	mPitch = std::clamp(mPitch + angInput.x * mCameraSensitivity, -89.0f, 89.0f);

	RMath::Vector3 targetRotation = RMath::Vector3();
	targetRotation.x = cos(RMath::ToRadians(mYaw)) * cos(RMath::ToRadians(mPitch));
	targetRotation.y = sin(RMath::ToRadians(mPitch));
	targetRotation.z = sin(RMath::ToRadians(mYaw)) * cos(RMath::ToRadians(mPitch));

	mTransform.Translate(translateDelta);
	mTransform.mOrientation = RMath::Quaternion::LookIn(targetRotation, RMath::Vector3::Up());

	mCameraTarget = mTransform.GetWorldPosition() + mTransform.GetForwardVector();
}