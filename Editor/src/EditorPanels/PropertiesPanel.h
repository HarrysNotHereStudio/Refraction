#pragma once

#include <EditorPanels/AEditorPanel.h>

namespace Refraction::Editor::Panels {
	class PropertiesPanel : public AEditorPanel {
	public:
		PropertiesPanel();
		~PropertiesPanel() = default;

		inline void Init() override {}
		void OnDraw() override;
		inline void OnEvent(Common::Ref<Events::Event> event) override {}

	private:

	};
}
