#pragma once

#include <string>
#include <deque>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include <Core/Common.h>

namespace RefractionEditor::Platform {
	class AImGuiImpl {
	public:
		static ImGuiStyle GetDefaultStyle();

		AImGuiImpl();

		virtual void Init() = 0;
		void Draw();
		void GetGuiInputState(Refraction::Enums::WindowInputState* inputState);
		void HideMouse();

	protected:
		virtual void BeginDraw() = 0;
		virtual void EndDraw() = 0;

	private:
		void DrawMenu();
		void DrawRibbon();
		void DrawExplorer();
		void DrawAssetDrawer();
	};
}