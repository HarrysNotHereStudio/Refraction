#include <imgui/imgui.h>

#include <EditorState.h>

#include "PropertiesPanel.h"

namespace Refraction::Editor::Panels {
	PropertiesPanel::PropertiesPanel() {

	}

	void PropertiesPanel::OnDraw() {
		ImGui::SetNextWindowSizeConstraints({ 350, 50 }, { FLT_MAX, FLT_MAX });
		ImGui::Begin("Properties");
		if (EditorState::Temp.SimulatingGame) ImGui::BeginDisabled();


	}
}
