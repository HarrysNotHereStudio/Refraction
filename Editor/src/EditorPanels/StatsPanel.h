#pragma once

#include <deque>

#include <EditorPanels/AEditorPanel.h>

namespace Refraction::Editor::Panels {
	class StatsPanel : public AEditorPanel {
	public:
		using AEditorPanel::AEditorPanel;
		~StatsPanel() = default;

		void Init() override;
		void OnDraw() override;
		void OnEvent(Common::Ref<Events::Event> event) override {}

	private:
		std::deque<float> mDeltaHistory = {};
	};
}
