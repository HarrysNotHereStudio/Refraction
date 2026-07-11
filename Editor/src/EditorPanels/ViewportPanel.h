#pragma once

#include <EditorPanels/AEditorPanel.h>

namespace Refraction::Editor::Panels {
	class ViewportPanel : public AEditorPanel {
	public:
		ViewportPanel(Common::Ref<Events::AEventDispatcher> eventDispatcher);
		~ViewportPanel() = default;

		inline void Init() override;
		void OnDraw() override;
		inline void OnEvent(Common::Ref<Events::Event> event) override;
	private:
		Common::Ref<Events::AEventDispatcher> mEventDispatcher;
		Common::Ref<Refraction::Assets::Texture> mFrame;
		Math::Rect mLastViewportRect;
		Math::Rect mViewportRect;
		bool mFirstDraw = true;
	};
}
