#pragma once

#include <Classes/Assets/Image.h>
#include <EditorGUI/Panels/AEditorPanel.h>

namespace Refraction::Editor::GUI {
	class ViewportPanel : public AEditorPanel {
	public:
		using AEditorPanel::AEditorPanel;
		~ViewportPanel() = default;

		void Init() override;
		void OnDraw() override;
		void OnEvent(Common::Ref<Events::Event> event) override;
	private:
		Common::Ref<Assets::Image> mFrame;
		Math::Rect mLastViewportRect;
		Math::Rect mViewportRect;
		bool mFirstDraw = true;
	};
}
