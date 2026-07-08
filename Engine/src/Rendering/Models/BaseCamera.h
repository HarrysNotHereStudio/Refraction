#pragma once

#include <algorithm>

#include <Core/Common.h>
#include <Math/Transform.h>
#include <Math/Vector3.h>
#include <Math/Frustum.h>
#include <Math/Matrices.h>

class BaseCamera
{
public:
	static BaseCamera* ActiveCamera;

	Refraction::Math::Transform mTransform;
	Refraction::Math::Frustum mFrustum;

	BaseCamera();

	void Move(Refraction::Math::Vector3 dirInput, Refraction::Math::Vector3 angInput);
	Refraction::Math::Matrix4 GetViewMatrix() const { return Refraction::Math::Matrix4::LookAt(mTransform.GetWorldPosition(), mCameraTarget, mTransform.GetUpVector()); };

private:
	Refraction::Math::Vector3 mCameraTarget;
	float mYaw = 0;
	float mPitch = 0;
	float mCameraSpeed = 1;
	float mCameraSensitivity = 1;
};