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
		EditorLayer(Common::Shared<Events::AEventDispatcher> eventDispatcher, Common::Shared<Engine::Project> projectInstance, Common::Shared<Engine::Platform::AWindow> window, Common::Shared<Editor::Platform::AImGuiImpl> imGuiImpl);

		void OnAttach() override;
		void OnDetach() override;
		void OnPass() override;
		void OnEvent(Common::Shared<Events::Event> event) override;

	private:
		Common::Shared<Events::AEventDispatcher> mEventDispatcher;
		Common::Shared<Engine::Project> mProjectInstance;
		Common::Shared<Engine::Platform::AWindow> mWindow;
		Common::Shared<Editor::Platform::AImGuiImpl> mImGuiImpl;

		std::vector<Common::URef<GUI::AEditorPanel>> mEditorPanels;
	};
}