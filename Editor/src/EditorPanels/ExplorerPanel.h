#pragma once

#include <EditorPanels/AEditorPanel.h>

namespace Refraction::Editor::Panels {
	class ExplorerPanel : public AEditorPanel {
	public:
		using AEditorPanel::AEditorPanel;
		~ExplorerPanel() = default;

		void Init() override {}
		void OnDraw() override;
		void OnEvent(Common::Ref<Events::Event> event) override {}

	private:
		// TODO: filter objects in Explorer
		ImGuiTextFilter mFilter;
	};
}
