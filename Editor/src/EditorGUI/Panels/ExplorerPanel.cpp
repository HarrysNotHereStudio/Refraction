#include <format>

#include <imgui/imgui.h>

#include <Classes/ClassHeaders.h>
#include <EditorState.h>

#include "ExplorerPanel.h"

namespace Refraction::Editor::GUI {
	static void MakeTree(Common::Shared<Objects::AObject> obj) {
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

		auto& currentlySelected = EditorState::Temp.SelectedObject;
		if (currentlySelected && (obj->GetUUID().AsInt() == currentlySelected->GetUUID().AsInt())) flags |= ImGuiTreeNodeFlags_Selected;
		if (obj->GetChildren()->size() < 1) flags |= ImGuiTreeNodeFlags_Leaf;

		auto isOpen = ImGui::TreeNodeEx(std::format("{}##{}ExplorerTreeNode", obj->mInstanceName, obj->GetUUID().AsString()).c_str(), flags);
		if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
			currentlySelected = obj;
			if (ImGui::BeginPopup("ExplorerItemPopup")) {
				if (ImGui::Button("Delete")) {
					obj->mParent->RemoveChild(obj->GetUUID());
				}
				ImGui::EndPopup();
			}
		}
		if (isOpen) {
			for (auto& child : *obj->GetChildren()) {
				MakeTree(child);
			}
			ImGui::TreePop();
		}

	}

	void ExplorerPanel::OnDraw() {
		if (!EditorState::Temp.PanelExplorerVisible) return;
		ImGui::SetNextWindowSizeConstraints({ 150, 50 }, { FLT_MAX, FLT_MAX });
		ImGui::Begin("Explorer", &EditorState::Temp.PanelExplorerVisible);
		if (EditorState::Temp.SimulatingGame) ImGui::BeginDisabled();

		auto& project = EditorState::Temp.ProjectInstance;
		if (!project->IsLoaded()) {
			ImGui::Text("No project loaded");
		} else {
			for (auto& scene : project->GetScenes()) {
				MakeTree(scene);
			}
			for (auto& globalObj : project->GetGlobalObjects()) {
				MakeTree(globalObj);
			}
		}
		if (ImGui::BeginPopupContextWindow("ExplorerPopup")) {
			if (!project->IsLoaded()) ImGui::BeginDisabled();
			if (ImGui::BeginMenu("Add Object...")) {
				if (ImGui::MenuItem("Empty Object")) {
					auto newObj = Common::NewShared<Objects::AObject>();
					EditorState::Temp.ProjectInstance->GetActiveScene()->AddChild(newObj);
				}
				if (ImGui::MenuItem("Basic Object")) {
					auto newObj = Common::NewShared<Objects::BasicObject>();
					EditorState::Temp.ProjectInstance->GetActiveScene()->AddChild(newObj);
				}
				if (ImGui::MenuItem("Scene")) {
					EditorState::Temp.ProjectInstance->NewScene();
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
			if (!project->IsLoaded()) ImGui::EndDisabled();
		}

		if (EditorState::Temp.SimulatingGame) ImGui::EndDisabled();
		ImGui::End();
	}
}
