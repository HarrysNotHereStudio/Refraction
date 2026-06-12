#pragma once

#include <EditorPlatform/AImGuiImpl.h>

namespace RefractionEditor::Platform::OpenGL {
	class ImGuiImpl : public AImGuiImpl {
	public:
		ImGuiImpl();

		void Init() override;
	private:
		void BeginDraw() override;
		void EndDraw() override;
	};
}