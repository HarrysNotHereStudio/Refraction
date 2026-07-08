#pragma once

#include <Math/Common.h>
#include <Math/Vector3.h>
#include <Math/Quaternion.h>

namespace Refraction::Math {
	constexpr int SpatialCellSize = 128;
	struct SpatialPosition {
		Vector3 GridIndex = Vector3::Zero();
		Vector3 CellPosition = Vector3::Zero();

		inline Vector3 ToWorld() const { return CellPosition + (GridIndex * SpatialCellSize); }
		void Translate(Vector3 delta);
	};

	class Transform {
	public:
		SpatialPosition mSpatialPosition;
		Quaternion mOrientation;
		Vector3 mScale;

		Transform();
		Transform(const Vector3& pos);

		// Creates a Transform looking at a target
		static Transform FromLookAt(const Vector3& eye, Vector3 target, Vector3 targetUp = Vector3::Up());

		inline void Translate(Vector3 delta) { mSpatialPosition.Translate(delta); }
		// Rotate using an angle (degrees) axis
		void Rotate(float angle, Vector3 axis);
		// Rotate using Euler angles (degrees)
		void Rotate(Vector3 delta);
		// Rotate using a Quaternion
		void Rotate(Quaternion delta);
		void Scale(Vector3 delta);
		// Rotates the Transform to look at a target
		void LookAt(Vector3 target, Vector3 targetUp = Vector3::Up());

		// Generates the Transform's matrix
		Matrix4 GetTransform() const;
		inline Vector3 GetWorldPosition() const { return mSpatialPosition.ToWorld(); }
		inline Vector3 GetForwardVector() const { return mOrientation * Vector3::Front(); }
		inline Vector3 GetRightVector() const { return mOrientation * Vector3::Right(); }
		inline Vector3 GetUpVector() const { return mOrientation * Vector3::Up(); }

		inline std::string ToString(PrintFormatArgs fmtArgs = PrintFormatArgs()) const {
			auto gridIndexStr = mSpatialPosition.GridIndex.ToString({ .AsInt = true, .Pretty = fmtArgs.Pretty });
			auto cellPosStr = mSpatialPosition.CellPosition.ToString({ .AsInt = fmtArgs.AsInt, .Pretty = fmtArgs.Pretty });
			auto orientationStr = mOrientation.ToString({ .AsInt = fmtArgs.AsInt, .Pretty = fmtArgs.Pretty });
			auto scaleStr = mScale.ToString({ .AsInt = fmtArgs.AsInt, .Pretty = fmtArgs.Pretty });
			if (fmtArgs.Pretty) {
				return std::string("Position: [GridIndex: " + gridIndexStr + ", CellPosition: " + cellPosStr + "]\nOrientation: " + orientationStr + "\nScale: " + scaleStr);
			} else {
				return std::string("{[" + gridIndexStr + ", " + cellPosStr + "], " + orientationStr + ", " + scaleStr + "}");
			}
		}
	};
}