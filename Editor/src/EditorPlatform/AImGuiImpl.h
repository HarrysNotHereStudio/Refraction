#pragma once

#include <string>
#include <deque>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include <Core/Common.h>
#include <Math/Vector3.h>
#include <Classes/Objects/AObject.h>
#include <Platform/AWindow.h>
#include <Interface/Project.h>

constexpr int ImGuiImpl_DeltaHistoryMax = 90;


namespace Refraction::Editor::Platform {
	class AImGuiImpl {
	public:
		Common::Ref<Objects::AObject> mSelectedObject = nullptr;

		static ImGuiStyle GetDefaultStyle();

		AImGuiImpl(Common::Ref<Engine::Platform::AWindow> window, Common::Ref<Engine::Project> project);

		virtual void Init() = 0;
		virtual void BeginDraw() = 0;
		virtual void EndDraw() = 0;

		void HideMouse();
		void UpdateInputState();
		void DrawMenu();
		void DrawRibbon();

		inline bool ShouldQuit() const { return mShouldQuit; }

	protected:
		Common::Ref<Engine::Platform::AWindow> mWindow;
		Common::Ref<Engine::Project> mProject;

		virtual void CloseWindow() = 0;

	private:
		std::deque<float> mDeltaHistory = {};
		bool mShouldQuit = false;

		void DrawAssetDrawer();

		void DrawDebugInfoWindow();
	};
}