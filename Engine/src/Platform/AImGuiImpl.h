#pragma once

#include <Core/Constants.h>

namespace Refraction::Platform {
	class AImGuiImpl {
	public:
		struct {
			float fps;
			float deltaTime;
			float elapsedTime;
			char* inputStateStr;
		} mDebugValues;

		virtual void Draw() = 0;
		virtual void GetGuiInputState(Refraction::Enums::WindowInputState* inputState) = 0;
		virtual void HideMouse() = 0;
	};
}