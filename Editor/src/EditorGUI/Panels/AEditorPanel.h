#pragma once

#include <Core/Common.h>
#include <Core/EventSystem.h>
#include <Platform/AWindow.h>
#include <ImGuiExtension.h>

namespace Refraction::Editor::GUI {
	class AEditorPanel {
	public:
		AEditorPanel(Common::SRef<Events::AEventDispatcher> eventDispatcher, Common::SRef<Engine::Platform::AWindow> window)
			: mEventDispatcher(eventDispatcher), mWindow(window) {}
		virtual ~AEditorPanel() = default;

		virtual void Init() = 0;
		virtual void OnDraw() = 0;
		virtual void OnEvent(Common::SRef<Events::Event> event) = 0;
	protected:
		Common::SRef<Events::AEventDispatcher> mEventDispatcher;
		Common::SRef<Engine::Platform::AWindow> mWindow;
	};
}
