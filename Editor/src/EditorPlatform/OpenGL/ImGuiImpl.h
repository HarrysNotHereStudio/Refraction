#pragma once

#include <EditorPlatform/AImGuiImpl.h>

namespace Refraction::Editor::Platform::OpenGL {
	class ImGuiImpl : public AImGuiImpl {
	public:
		using AImGuiImpl::AImGuiImpl;

		void Init() override;
	private:
		void BeginDraw() override;
		void EndDraw() override;
		void CloseWindow() override;
	};
}