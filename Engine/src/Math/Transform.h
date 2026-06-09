#pragma once

#include <Core/Utilities.h>
#include <Math/Common.h>
#include <Math/Vector3.h>
#include <Math/Quaternion.h>

namespace Refraction::Math {
	class Transform {
	public:
		Vector3 mPosition;
		Quaternion mOrientation;
		Vector3 mScale;

		Transform();

		void Translate(Vector3 delta);
		void Rotate(float angle, Vector3 axis);
		void Rotate(Vector3 delta);
		void Rotate(Quaternion delta);
		void Scale(Vector3 delta);

		Matrix4 GetTransform() const;
		Vector3 GetForwardVector() const { return mOrientation * Vector3::Front(); };
		Vector3 GetRightVector() const { return mOrientation * Vector3::Right(); };
		Vector3 GetUpVector() const { return mOrientation * Vector3::Up(); };

		// Utility
		static bool AreQuaternionsSimilar(Quaternion quatA, Quaternion quatB) {
			return (abs(quatA.Dot(quatB) - 1.0) < 0.001);
		}
		static Quaternion LookAt(const Vector3& eye, Vector3 target, Vector3 targetUp) {
			Vector3 direction = target - eye;

			auto rot1 = Quaternion::RotationBetweenEulerAngles(Vector3::Z(), direction);

			Vector3 right = direction.Cross(targetUp);
			targetUp = right.Cross(direction);

			Vector3 newUp = Vector3::Up() * rot1;
			auto rot2 = Quaternion::RotationBetweenEulerAngles(newUp, targetUp);

			return rot2 * rot1;
		}
	};
}