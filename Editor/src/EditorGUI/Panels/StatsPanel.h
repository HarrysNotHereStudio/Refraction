#pragma once

#include <deque>

#include <EditorGUI/Panels/AEditorPanel.h>

namespace Refraction::Editor::GUI {
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
