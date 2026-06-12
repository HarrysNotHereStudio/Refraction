#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp> 

#include "Transform.h"

namespace RUtil = Refraction::Utilities;

namespace Refraction::Math {
	void SpatialPosition::Translate(Vector3 delta) {
		CellPosition += delta;
		while (CellPosition.x < 0) {
			GridIndex.x -= 1;
			CellPosition.x += SpatialCellSize;
		}
		while (CellPosition.x > SpatialCellSize) {
			GridIndex.x += 1;
			CellPosition.x -= SpatialCellSize;
		}
		while (CellPosition.y < 0) {
			GridIndex.y -= 1;
			CellPosition.y += SpatialCellSize;
		}
		while (CellPosition.y > SpatialCellSize) {
			GridIndex.y += 1;
			CellPosition.y -= SpatialCellSize;
		}
		while (CellPosition.z < 0) {
			GridIndex.z -= 1;
			CellPosition.z += SpatialCellSize;
		}
		while (CellPosition.z > SpatialCellSize) {
			GridIndex.z += 1;
			CellPosition.z -= SpatialCellSize;
		}
	}


	Transform::Transform() {
		mSpatialPosition.GridIndex = Vector3(0);
		mSpatialPosition.CellPosition = Vector3(0.0f);
		mOrientation = Quaternion();
		mScale = Vector3(1.0f);
	}

	Transform::Transform(const Vector3& pos) : Transform() {
		Translate(pos);
	}

	Transform Transform::FromLookAt(const Vector3& eye, Vector3 target, Vector3 targetUp) {
		Transform result(eye);
		result.LookAt(target, targetUp);
		return result;
	}

	void Transform::Rotate(float angle, Vector3 axis) {
		auto rotQ = Quaternion::FromAxisAngle(angle, axis);
		auto conjQ = rotQ.Conjugate();
		mOrientation = rotQ * mOrientation * conjQ;
		mOrientation.ResetNANs();
	}

	void Transform::Rotate(Vector3 delta) {
		mOrientation *= Quaternion::FromEulerAngles(delta);
		mOrientation.ResetNANs();
	}

	void Transform::Rotate(Quaternion delta) {
		mOrientation *= delta;
		mOrientation.ResetNANs();
	}

	void Transform::Scale(Vector3 delta) {
		mScale += delta;
	}

	void Transform::LookAt(Vector3 target, Vector3 targetUp) {
		Rotate(Quaternion::LookAt(GetWorldPosition(), target, targetUp));
	}

	Matrix4 Transform::GetTransform() const {
		glm::mat4 transform = glm::mat4(1.0f);
		glm::vec3 rotation = RUtil::NativeToGLMVec3(mOrientation.ToEulerAngles());
		transform = glm::translate(transform, RUtil::NativeToGLMVec3(GetWorldPosition()));
		transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, RUtil::NativeToGLMVec3(mScale));
		//Matrix4 transform = Matrix4();
		//transform = transform.Translate(mPosition);
		//transform = transform.Rotate(mOrientation);
		//transform = transform.Scale(mScale);
		return RUtil::GLMToNativeMat4(transform);
	}
}
