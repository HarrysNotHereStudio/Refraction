#pragma once

#include <string>

#include <Core/Constants.h>
#include <Math/Vector3.h>
#include <EngineClasses/Objects/AObject.h>

namespace Refraction::Platform {
	class AImGuiImpl {
	public:
		struct {
			float fps = 0;
			float deltaTime = 0;
			float elapsedTime = 0;
			std::string inputStateStr = "None";
			Math::Vector3 lastMousePos = Math::Vector3(0.0f);
			Math::Vector3 cameraGridIndex = Math::Vector3(0);
			Math::Vector3 cameraCellPos = Math::Vector3(0.0f);
			Math::Vector3 cameraWorldPos = Math::Vector3(0.0f);
		} mDebugValues;
		Common::Ref<Objects::AObject> mSelectedObject = nullptr;

		virtual void Draw() = 0;
		virtual void GetGuiInputState(Refraction::Enums::WindowInputState* inputState) = 0;
		virtual void HideMouse() = 0;
	};
}