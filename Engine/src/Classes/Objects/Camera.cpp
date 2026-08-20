#include <json.hpp>

#include <Settings.h>
#include <Classes/ClassSerialiser.h>

#include "Camera.h"

namespace Refraction::Objects {
	Common::SRef<Camera> Camera::ActiveCamera = nullptr;
	Math::Frustum defaultFrustum = Math::Frustum(Math::Vector2(128), 70.0f, 0.001f, 10000.0f);

	Camera::Camera() : mFrustum(defaultFrustum) {
		mInstanceName = "Camera";
	}

	void Camera::ProcessInput(Math::Vector3 dirInput, Math::Vector3 angInput) {

		// Normalize inputs
		if (dirInput.Magnitude() > 0) dirInput.Normalise();

		auto& cameraSensitivity = Settings::CurrentSettings->Controls.CameraSensitivity;
		auto& cameraSpeed = Settings::CurrentSettings->Controls.CameraSpeed;

		// Move camera
		Math::Vector3 translateDelta = Math::Vector3();
		if (abs(dirInput.z) > 0.0f) translateDelta += mTransform.GetForwardVector() * (dirInput.z * cameraSpeed);
		if (abs(dirInput.y) > 0.0f) translateDelta += mTransform.GetUpVector() * (dirInput.y * cameraSpeed);
		if (abs(dirInput.x) > 0.0f) translateDelta += mTransform.GetRightVector() * (dirInput.x * cameraSpeed);

		// Rotate camera
		mTransform.mOrientation.mYaw += angInput.y * cameraSensitivity;
		mTransform.mOrientation.mPitch = std::clamp(mTransform.mOrientation.mPitch + angInput.x * cameraSensitivity, -89.0f, 89.0f);

		//Math::Vector3 targetRotation = Math::Vector3();
		//targetRotation.x = cos(Math::ToRadians(mYaw)) * cos(Math::ToRadians(mPitch));
		//targetRotation.y = sin(Math::ToRadians(mPitch));
		//targetRotation.z = sin(Math::ToRadians(mYaw)) * cos(Math::ToRadians(mPitch));

		mTransform.Translate(translateDelta);
		//mTransform.mOrientation = Math::Quaternion::LookIn(targetRotation, Math::Vector3::Up()).ToEulerAngles();

		mCameraTarget = mTransform.GetWorldPosition() + mTransform.GetForwardVector();
	}

	std::string Camera::Serialise() {
		return Utilities::ClassSerialiser::TryAppendJSON(AObject::Serialise(), [&](nlohmann::json& json) {
			json["Frustum"] = Utilities::ClassSerialiser::Serialise(mFrustum);
		});
	}
	void Camera::Deserialise(std::string serialised) {
		AObject::Deserialise(serialised);
		Utilities::ClassSerialiser::TryParseJSON(serialised, [&](nlohmann::json& json) {
			mFrustum = Utilities::ClassSerialiser::DeserialiseFrustum(json.at("Frustum"));
		});
	}
}