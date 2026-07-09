#pragma once

#include <Core/Common.h>
#include <Core/EventSystem.h>
#include <ImGuiExtension.h>

namespace Refraction::Editor::Panels {
	class AEditorPanel {
	public:
		virtual ~AEditorPanel() = default;

		virtual void Init() = 0;
		virtual void OnDraw() = 0;
		virtual void OnEvent(Common::Ref<Events::Event> event) = 0;
	};
}
