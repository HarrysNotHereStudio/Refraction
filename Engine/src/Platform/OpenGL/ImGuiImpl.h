#pragma once

#include <Platform/AImGuiImpl.h>

namespace Refraction::Platform::OpenGL {
	class ImGuiImpl : public AImGuiImpl {
	public:
		ImGuiImpl();

		void Draw() override;
		void GetGuiInputState(Refraction::Enums::WindowInputState* inputState) override;
		void HideMouse() override;
	};
}