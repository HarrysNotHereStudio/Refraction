#pragma once

#include <EditorGUI/Panels/AEditorPanel.h>

namespace Refraction::Editor::GUI {
	class LogPanel : public AEditorPanel {
	public:
		using AEditorPanel::AEditorPanel;
		LogPanel(Common::Ref<Events::AEventDispatcher> eventDispatcher, Common::Ref<Engine::Platform::AWindow> window);
		~LogPanel() = default;

		void Init() override {}
		void OnDraw() override;
		void OnEvent(Common::Ref<Events::Event> event) override {}

	private:
		std::vector<std::tuple<Log::Colour, std::string, bool>> mHistory = {};
		bool mShouldAutoscroll = true;
	};
}
