#pragma once

#include <EditorPanels/AEditorPanel.h>

namespace Refraction::Editor::Panels {
	class LiveCollabPanel : public AEditorPanel {
	public:
		using AEditorPanel::AEditorPanel;
		~LiveCollabPanel() = default;

		void Init() override {}
		void OnDraw() override;
		void OnEvent(Common::Ref<Events::Event> event) override {}

	private:

	};
}
