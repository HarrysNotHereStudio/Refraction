#include <Rendering/RenderLayer.h>
#include <EditorState.h>
#include <EditorPanels/ExplorerPanel.h>
#include <EditorPanels/PropertiesPanel.h>
#include <EditorPanels/ViewportPanel.h>

#include "EditorLayer.h"

namespace Refraction::Editor {
	EditorLayer::EditorLayer(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Engine::Project> projectInstance, Common::Ref<Engine::Platform::AWindow> window, Common::Ref<Editor::Platform::AImGuiImpl> imGuiImpl)
		: mEventDispatcher(eventDispatcher), mProjectInstance(projectInstance), mWindow(window), mImGuiImpl(imGuiImpl) {
		mEditorPanels.push_back(Common::NewURef<Panels::ExplorerPanel>());
		mEditorPanels.push_back(Common::NewURef<Panels::PropertiesPanel>());
		mEditorPanels.push_back(Common::NewURef<Panels::ViewportPanel>(eventDispatcher));
	}

	void EditorLayer::OnAttach() {
		if (EditorState::Deserialise()) {
			mWindow->SetRect(EditorState::Persistent.WindowRect);
		} else {
			Log::Editor.Warn("Failed to deserialise EditorState");
		}
		EditorState::Temp.ActiveProject = mProjectInstance;
		mWindow->mIgnoreWindowResize = true;
		mImGuiImpl->Init();

		for (auto& panel : mEditorPanels) {
			panel->Init();
		}
	}

	void EditorLayer::OnDetach() {
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
		ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + 16));
		ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y - 16));
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

		mImGuiImpl->DrawDebugInfoWindow();
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

