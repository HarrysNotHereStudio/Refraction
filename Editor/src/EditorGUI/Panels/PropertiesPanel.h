#pragma once

#include <EditorGUI/Panels/AEditorPanel.h>

namespace Refraction::Editor::GUI {
	class PropertiesPanel : public AEditorPanel {
	public:
		using AEditorPanel::AEditorPanel;
		~PropertiesPanel() = default;

		void Init() override {}
		void OnDraw() override;
		void OnEvent(Common::SRef<Events::Event> event) override {}

	private:

	};
}
