#include <Rendering/RenderLayer.h>
#include <EditorState.h>
#include <EditorTheme.h>
#include <EditorPanels/ExplorerPanel.h>
#include <EditorPanels/PropertiesPanel.h>
#include <EditorPanels/ViewportPanel.h>
#include <EditorPanels/LogPanel.h>
#include <EditorPanels/StatsPanel.h>

#include "EditorLayer.h"

constexpr int ImGuiMenuHeight = 16;
constexpr int ImGuiRibbonHeight = 48;

namespace Refraction::Editor {
	EditorLayer::EditorLayer(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Engine::Project> projectInstance, Common::Ref<Engine::Platform::AWindow> window, Common::Ref<Editor::Platform::AImGuiImpl> imGuiImpl)
		: mEventDispatcher(eventDispatcher), mProjectInstance(projectInstance), mWindow(window), mImGuiImpl(imGuiImpl) {
		mEditorPanels.push_back(Common::NewURef<Panels::ExplorerPanel>(eventDispatcher, mWindow));
		mEditorPanels.push_back(Common::NewURef<Panels::PropertiesPanel>(eventDispatcher, mWindow));
		mEditorPanels.push_back(Common::NewURef<Panels::ViewportPanel>(eventDispatcher, mWindow));
		mEditorPanels.push_back(Common::NewURef<Panels::LogPanel>(eventDispatcher, mWindow));
		mEditorPanels.push_back(Common::NewURef<Panels::StatsPanel>(eventDispatcher, mWindow));
	}

	void EditorLayer::OnAttach() {
		if (EditorState::Deserialise()) {
			mWindow->SetRect(EditorState::Persistent.WindowRect);
		} else {
			Log::Editor.Warn("Failed to deserialise EditorState");
		}
		EditorState::Temp.ProjectInstance = mProjectInstance;
		mWindow->mIgnoreWindowResize = true;
		mImGuiImpl->Init();

		auto themeFilePath = FileHandling::GetWorkingDirectory() / ("EditorTheme" + std::string(REFRACTION_THEME_EXTENSION));
		if (std::filesystem::exists(themeFilePath)) {
			EditorTheme::LoadFromFile(themeFilePath);
			EditorTheme::ApplyTheme();
		} else {
			EditorTheme::LoadDefault();
			EditorTheme::ApplyTheme();
		}

		for (auto& panel : mEditorPanels) {
			panel->Init();
		}
	}

	void EditorLayer::OnDetach() {
		EditorState::Temp.SelectedObject = nullptr;
		EditorState::Temp.ProjectInstance = nullptr;
		EditorState::Persistent.WindowRect = mWindow->GetRect();
		if (!EditorState::Serialise()) {
			Log::Editor.Warn("Failed to serialise EditorState");
		}
	}

	void EditorLayer::OnPass() {
		auto activeScene = mProjectInstance->GetActiveScene();
		if (activeScene) {
			auto sceneChildren = activeScene->GetChildren();
			if (sceneChildren->size() > 0) {
				mImGuiImpl->mSelectedObject = sceneChildren->at(0);
			}
		}
		mImGuiImpl->BeginDraw();

		mImGuiImpl->DrawMenu();
		mImGuiImpl->DrawRibbon();

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		//Log::Editor.Info(Math::Vector2(viewport->Size.x, viewport->Size.y).ToString());
		auto totalOffset = ImGuiMenuHeight + ImGuiRibbonHeight;
		ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + totalOffset));
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - totalOffset));
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (!mProjectInstance->IsLoaded()) {
			// TODO: Launcher
		} else {

		}
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin("##DockspaceArea", nullptr, windowFlags);
		ImGui::PopStyleVar(2);

		ImGuiStyle& style = ImGui::GetStyle();
		float minSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f;
		ImGui::DockSpace(ImGui::GetID("EditorDockspace"));
		style.WindowMinSize.x = minSizeX;

		for (auto& panel : mEditorPanels) {
			panel->OnDraw();
		}
		ImGui::End();

		mImGuiImpl->EndDraw();
		mImGuiImpl->UpdateInputState();
		
		if (mImGuiImpl->ShouldQuit()) {
			mEventDispatcher->Dispatch(Common::NewRef<Events::ProgramCloseEvent>());
		}

		if (EditorState::Temp.ViewportHovered) {
			mWindow->mInputState = Engine::Platform::WindowInputState::VIEWPORT;
		} else {
			mWindow->mInputState = Engine::Platform::WindowInputState::GUI;
		}
	}

	void EditorLayer::OnEvent(Common::Ref<Events::Event> event) {
		if (auto e = Common::AsA<Events::ViewportResizedEvent>(event)) {
		} else if (auto e = Common::AsA<Events::ProgramCloseEvent>(event)) {
			// TODO: close confirmation modal
		}

		for (auto& panel : mEditorPanels) {
			panel->OnEvent(event);
			if (event->Consumed()) break;
		}
	}
}

