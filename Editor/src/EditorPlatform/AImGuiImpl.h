#pragma once

#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include <Core/Common.h>
#include <Math/Vector.h>
#include <Classes/Objects/AObject.h>
#include <Platform/AWindow.h>
#include <Interface/Project.h>

namespace Refraction::Editor::Platform {
	class AImGuiImpl {
	public:
		Common::SRef<Objects::AObject> mSelectedObject = nullptr;

		AImGuiImpl(Common::SRef<Engine::Platform::AWindow> window);

		virtual void Init() = 0;
		virtual void BeginDraw() = 0;
		virtual void EndDraw() = 0;

		void HideMouse();
		void UpdateInputState();
		void DrawMenu();
		void DrawRibbon();
		void DrawStatsBar();

		inline bool ShouldQuit() const { return mShouldQuit; }

	protected:
		Common::SRef<Engine::Platform::AWindow> mWindow;

		virtual void CloseWindow() = 0;

	private:
		bool mShouldQuit = false;

		float mMenuHeight = 8;
		float mRibbonHeight = 48;
		float mStatsBarHeight = 8;

		bool mShowDemoWindow = false;
		bool mShowThemeEditor = false;
		bool mQuitModal = false;
		bool mCloseProjectModal = false;
	};
}