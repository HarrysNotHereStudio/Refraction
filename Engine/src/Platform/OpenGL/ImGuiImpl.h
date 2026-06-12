#pragma once

#include <Platform/AImGuiImpl.h>

namespace Refraction::Platform::OpenGL {
	class ImGuiImpl : public AImGuiImpl {
	public:
		ImGuiImpl();

		void BeginDraw() override;
		void EndDraw() override;
	};
}