#pragma once

#include <Classes/Assets/Texture.h>
#include <EditorPanels/AEditorPanel.h>

namespace Refraction::Editor::Panels {
	class ViewportPanel : public AEditorPanel {
	public:
		using AEditorPanel::AEditorPanel;
		~ViewportPanel() = default;

		void Init() override;
		void OnDraw() override;
		void OnEvent(Common::Ref<Events::Event> event) override;
	private:
		Common::Ref<Refraction::Assets::Texture> mFrame;
		Math::Rect mLastViewportRect;
		Math::Rect mViewportRect;
		bool mFirstDraw = true;
	};
}
