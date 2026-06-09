#pragma once

#include <string>

#include <Core/Constants.h>

namespace Refraction::Platform {
	class AImGuiImpl {
	public:
		struct {
			float fps = 0;
			float deltaTime = 0;
			float elapsedTime = 0;
			std::string inputStateStr = "None";
		} mDebugValues;

		virtual void Draw() = 0;
		virtual void GetGuiInputState(Refraction::Enums::WindowInputState* inputState) = 0;
		virtual void HideMouse() = 0;
	};
}