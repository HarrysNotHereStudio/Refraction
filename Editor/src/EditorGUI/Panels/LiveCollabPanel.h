#pragma once

#include <EditorGUI/Panels/AEditorPanel.h>

namespace Refraction::Editor::GUI {
	class LiveCollabPanel : public AEditorPanel {
	public:
		using AEditorPanel::AEditorPanel;
		~LiveCollabPanel() = default;

		void Init() override {}
		void OnDraw() override;
		void OnEvent(Common::SRef<Events::Event> event) override {}

	private:

	};
}
