#pragma once

#include <Core/LayerSystem.h>
#include <Core/EventSystem.h>
#include <Platform/AWindow.h>
#include <Interface/Project.h>
#include <EditorPlatform/OpenGL/ImGuiImpl.h>
#include <EditorGUI/Panels/AEditorPanel.h>

namespace Refraction::Editor {
	class EditorLayer : public Engine::ALayer {
	public:
		EditorLayer(Common::SRef<Events::AEventDispatcher> eventDispatcher, Common::SRef<Engine::Project> projectInstance, Common::SRef<Engine::Platform::AWindow> window, Common::SRef<Editor::Platform::AImGuiImpl> imGuiImpl);

		void OnAttach() override;
		void OnDetach() override;
		void OnPass() override;
		void OnEvent(Common::SRef<Events::Event> event) override;

	private:
		Common::SRef<Events::AEventDispatcher> mEventDispatcher;
		Common::SRef<Engine::Project> mProjectInstance;
		Common::SRef<Engine::Platform::AWindow> mWindow;
		Common::SRef<Editor::Platform::AImGuiImpl> mImGuiImpl;

		std::vector<Common::URef<GUI::AEditorPanel>> mEditorPanels;
	};
}