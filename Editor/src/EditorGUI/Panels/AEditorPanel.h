#pragma once

#include <Core/Common.h>
#include <Core/EventSystem.h>
#include <Platform/AWindow.h>
#include <ImGuiExtension.h>

namespace Refraction::Editor::GUI {
	class AEditorPanel {
	public:
		AEditorPanel(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Engine::Platform::AWindow> window)
			: mEventDispatcher(eventDispatcher), mWindow(window) {}
		virtual ~AEditorPanel() = default;

		virtual void Init() = 0;
		virtual void OnDraw() = 0;
		virtual void OnEvent(Common::Ref<Events::Event> event) = 0;
	protected:
		Common::Ref<Events::AEventDispatcher> mEventDispatcher;
		Common::Ref<Engine::Platform::AWindow> mWindow;
	};
}
