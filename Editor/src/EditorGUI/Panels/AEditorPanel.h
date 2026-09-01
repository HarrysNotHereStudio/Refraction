#pragma once

#include <Core/Common.h>
#include <Core/EventSystem.h>
#include <Platform/AWindow.h>
#include <ImGuiExtension.h>

namespace Refraction::Editor::GUI {
	class AEditorPanel {
	public:
		AEditorPanel(Common::Shared<Events::AEventDispatcher> eventDispatcher, Common::Shared<Engine::Platform::AWindow> window)
			: mEventDispatcher(eventDispatcher), mWindow(window) {}
		virtual ~AEditorPanel() = default;

		virtual void Init() = 0;
		virtual void OnDraw() = 0;
		virtual void OnEvent(Common::Shared<Events::Event> event) = 0;
	protected:
		Common::Shared<Events::AEventDispatcher> mEventDispatcher;
		Common::Shared<Engine::Platform::AWindow> mWindow;
	};
}
