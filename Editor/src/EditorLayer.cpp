#include <Rendering/RenderLayer.h>
#include <EditorState.h>

#include "EditorLayer.h"

namespace Refraction::Editor {
	EditorLayer::EditorLayer(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Engine::Project> projectInstance, Common::Ref<Engine::Platform::AWindow> window, Common::Ref<Editor::Platform::AImGuiImpl> imGuiImpl)
		: mEventDispatcher(eventDispatcher), mProjectInstance(projectInstance), mWindow(window), mImGuiImpl(imGuiImpl) {}

	void EditorLayer::OnAttach() {
		if (EditorState::Deserialise()) {
			mWindow->SetRect(EditorState::Persistent.WindowRect);
		} else {
			Log::Editor.Warn("Failed to deserialise EditorState");
		}
	}

	void EditorLayer::OnDetach() {
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
		mImGuiImpl->Draw();
		
		if (mImGuiImpl->ShouldQuit()) {
			mEventDispatcher->Dispatch(Common::NewRef<Events::ProgramCloseEvent>());
		}
	}

	void EditorLayer::OnEvent(Common::Ref<Events::Event> event) {
		if (auto e = Common::AsA<Events::ViewportResizedEvent>(event)) {
			EditorState::Persistent.WindowRect = e->mViewportRect;
		} else if (auto e = Common::AsA<Events::ProgramCloseEvent>(event)) {

		}
	}
}

