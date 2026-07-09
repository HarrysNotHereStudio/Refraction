#include <format>

#include <imgui/imgui.h>

#include <Classes/ClassHeaders.h>
#include <EditorState.h>

#include "ExplorerPanel.h"

namespace Refraction::Editor::Panels {
	static void MakeTree(Common::Ref<Objects::AObject> obj) {
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		auto& currentlySelected = EditorState::Temp.SelectedObject;
		if (currentlySelected && (obj->GetUUID().AsInt() == currentlySelected->GetUUID().AsInt())) flags |= ImGuiTreeNodeFlags_Selected;
		if (obj->GetChildren()->size() < 1) flags |= ImGuiTreeNodeFlags_Leaf;

		auto isOpen = ImGui::TreeNodeEx(obj->mInstanceName.c_str(), flags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			currentlySelected = obj;
		}
		if (isOpen) {
			for (auto& child : *obj->GetChildren()) {
				MakeTree(child);
			}
			ImGui::TreePop();
		}

	}

	ExplorerPanel::ExplorerPanel() {}

	void ExplorerPanel::OnDraw() {
		ImGui::SetNextWindowSizeConstraints({ 150, 50 }, { FLT_MAX, FLT_MAX });
		ImGui::Begin("Explorer");
		if (EditorState::Temp.SimulatingGame) ImGui::BeginDisabled();

		auto& project = EditorState::Temp.ActiveProject;
		if (!project) {
			ImGui::Text("No project loaded");
		} else {
			for (auto& scene : project->GetScenes()) {
				MakeTree(scene);
			}
			for (auto& globalObj : project->GetGlobalObjects()) {
				MakeTree(globalObj);
			}
		}
		ImGui::End();
	}
}
