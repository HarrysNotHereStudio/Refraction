#include <format>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include <Settings.h>
#include <Classes/Components/APhysics.h>
#include <EditorState.h>
#include <EditorTheme.h>
#include <ImGuiExtension.h>
#include <EditorPlatform/ADialogs.h>
#include <EditorPanels/ViewportPanel.h>
#include <EditorPanels/PropertiesPanel.h>
#include <EditorPanels/ExplorerPanel.h>

#include "AImGuiImpl.h"

namespace Refraction::Editor {
	using Engine::Platform::WindowInputState;

	Platform::AImGuiImpl::AImGuiImpl(Common::Ref<Engine::Platform::AWindow> window) : mWindow(window) {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowPadding = ImVec2(6, 6);
		style.FramePadding = ImVec2(2, 2);
		style.ItemSpacing = ImVec2(8, 2);
		style.ItemInnerSpacing = ImVec2(4, 2);
		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = 0;
		style.WindowRounding = 1;
		style.ChildRounding = 0;
		style.FrameRounding = 0;
		style.PopupRounding = 0;
		style.GrabRounding = 0;
		style.ScrollbarSize = 8;
		style.ScrollbarRounding = 0;
		style.ScrollbarPadding = 2;
		style.TabBorderSize = 0;
		style.TabBarBorderSize = 1;
		style.TabRounding = 0;
	}

	void Platform::AImGuiImpl::UpdateInputState() {
		if (ImGui::GetIO().WantCaptureMouse && !(mWindow->mInputState == WindowInputState::VIEWPORT && ImGui::GetIO().MouseDown[1])) {
			mWindow->mInputState = WindowInputState::GUI;
		} else if (mWindow->mInputState == WindowInputState::GUI) {
			mWindow->mInputState = WindowInputState::VIEWPORT;
		}
	}

	void Platform::AImGuiImpl::HideMouse() {
		ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	}

	void Platform::AImGuiImpl::DrawMenu() {
		auto& project = EditorState::Temp.ProjectInstance;

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New")) {
					project->New(Dialogs::SelectFolder("Select Project Folder"));
				}
				if (ImGui::MenuItem("Open")) {
					auto path = Dialogs::SelectFile(REFRACTION_PROJECT_EXTENSION, "Select Project File");
					EditorState::Persistent.RecentProjects.insert(path);
					project->Open(path);
				}
				if (ImGui::BeginMenu("Open Recent")) {
					for (auto& path : EditorState::Persistent.RecentProjects) {
						if (!std::filesystem::exists(path)) continue;
						if (ImGui::MenuItem(path.filename().string().c_str())) {
							project->Open(path);
						}
					}
					ImGui::EndMenu();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Save", "Ctrl+S", nullptr, project->IsLoaded())) {
					project->Save();
				}
				if (ImGui::MenuItem("Close", 0, nullptr, project->IsLoaded())) {
					project->Close();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Quit", "Alt+F4")) {
					mShouldQuit = true;
					CloseWindow();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {} // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
				if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
				if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				ImGui::MenuItem("Wireframe", 0, &Settings::CurrentSettings->Graphics.WireframeEnabled);
				ImGui::Separator();
				ImGui::MenuItem("Viewport", 0, &EditorState::Temp.PanelViewportVisible);
				ImGui::MenuItem("Properties", 0, &EditorState::Temp.PanelPropertiesVisible);
				ImGui::MenuItem("Explorer", 0, &EditorState::Temp.PanelExplorerVisible);
				ImGui::MenuItem("Statistics", 0, &EditorState::Temp.PanelStatisticsVisible);
				ImGui::MenuItem("Log", 0, &EditorState::Temp.PanelLogVisible);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Tools")) {
				ImGui::MenuItem("Theme Editor", 0, &mShowThemeEditor);
				ImGui::Separator();
				ImGui::MenuItem("ImGui Demo Window", 0, &mShowDemoWindow);
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		if (mShowDemoWindow) ImGui::ShowDemoWindow(&mShowDemoWindow);
		if (mShowThemeEditor) {
			EditorTheme::DrawThemeEditor(&mShowThemeEditor);
		};
	}

	void Platform::AImGuiImpl::DrawRibbon() {
	}
}
