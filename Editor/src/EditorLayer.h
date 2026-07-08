#pragma once

#include <Core/LayerSystem.h>
#include <Core/EventSystem.h>
#include <Platform/AWindow.h>
#include <Interface/Project.h>
#include <EditorPlatform/OpenGL/ImGuiImpl.h>

namespace Refraction::Editor {
	class EditorLayer : public Engine::ALayer {
	public:
		EditorLayer(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Engine::Project> projectInstance, Common::Ref<Engine::Platform::AWindow> window, Common::Ref<Editor::Platform::AImGuiImpl> imGuiImpl);

		void OnAttach() override;
		void OnDetach() override;
		void OnPass() override;
		void OnEvent(Common::Ref<Events::Event> event) override;

	private:
		Common::Ref<Events::AEventDispatcher> mEventDispatcher;
		Common::Ref<Engine::Project> mProjectInstance;
		Common::Ref<Engine::Platform::AWindow> mWindow;
		Common::Ref<Editor::Platform::AImGuiImpl> mImGuiImpl;
	};
}