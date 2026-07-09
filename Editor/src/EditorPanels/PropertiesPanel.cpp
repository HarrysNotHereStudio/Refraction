#include <format>

#include <imgui/imgui.h>

#include <Classes/ClassHeaders.h>
#include <EditorState.h>

#include "PropertiesPanel.h"

namespace Refraction::Editor::Panels {
	static void DrawTransformControls(Math::Transform& transform) {
		if (ImGui::TreeNode("Transform")) {
			// Handle rotation differently because it's not a Vector3
			auto objectRotation = transform.mOrientation.ToEulerAngles();
			auto displayedRotation = objectRotation;

			ImGuiSliderFlags flags = ImGuiSliderFlags_ColorMarkers;
 
			ImGui::Text("Position");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##Pos", ImGuiDataType_Float, &transform.mSpatialPosition.CellPosition.x, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
			ImGui::DragScalar("Y##Pos", ImGuiDataType_Float, &transform.mSpatialPosition.CellPosition.y, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
			ImGui::DragScalar("Z##Pos", ImGuiDataType_Float, &transform.mSpatialPosition.CellPosition.z, 0.01f, 0, 0, 0, flags);
			ImGui::PopItemWidth();
			ImGui::Text("Rotation");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##Rot", ImGuiDataType_Float, &displayedRotation.x, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
			ImGui::DragScalar("Y##Rot", ImGuiDataType_Float, &displayedRotation.y, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
			ImGui::DragScalar("Z##Rot", ImGuiDataType_Float, &displayedRotation.z, 0.01f, 0, 0, 0, flags);
			ImGui::Text(std::format("Quaternion value: {}", transform.mOrientation.ToString({ .AsInt = false, .Pretty = false })));
			ImGui::PopItemWidth();
			ImGui::Text("Scale");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##Scl", ImGuiDataType_Float, &transform.mScale.x, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
			ImGui::DragScalar("Y##Scl", ImGuiDataType_Float, &transform.mScale.y, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
			ImGui::DragScalar("Z##Scl", ImGuiDataType_Float, &transform.mScale.z, 0.01f, 0, 0, 0, flags);
			ImGui::PopItemWidth();

			ImGui::TreePop();
		}

	}

	static void DrawComponentProperties(Common::Ref<Components::AComponent> component) {
		std::string compFullTypeName = typeid(*component).name();
		auto compTypeName = compFullTypeName.substr(compFullTypeName.find_last_of(":") + 1);
		ImGui::Text("Component Type: " + compTypeName);
		ImGui::Text("UUID: " + component->GetUUID().AsString());

		if (auto casted = Common::AsA<Components::Mesh>(component)) {
			ImGui::Text("Mesh Source: " + casted->GetSource().string());
			ImGui::Spacing();
			DrawTransformControls(casted->mTransform);
		} else if (auto casted = Common::AsA<Components::APhysics>(component)) {
			ImGuiSliderFlags flags = ImGuiSliderFlags_ColorMarkers;
			if (ImGui::TreeNode("Linear Velocity")) {
				ImGui::PushItemWidth(100);
				ImGui::DragScalar("X##RigidPhysicsLinear", ImGuiDataType_Float, &casted->mLinearVelocity.x, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
				ImGui::DragScalar("Y##RigidPhysicsLinear", ImGuiDataType_Float, &casted->mLinearVelocity.y, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
				ImGui::DragScalar("Z##RigidPhysicsLinear", ImGuiDataType_Float, &casted->mLinearVelocity.z, 0.01f, 0, 0, 0, flags);
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Angular Velocity")) {
				ImGui::PushItemWidth(100);
				ImGui::DragScalar("X##RigidPhysicsAngular", ImGuiDataType_Float, &casted->mAngularVelocity.x, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
				ImGui::DragScalar("Y##RigidPhysicsAngular", ImGuiDataType_Float, &casted->mAngularVelocity.y, 0.01f, 0, 0, 0, flags); ImGui::SameLine();
				ImGui::DragScalar("Z##RigidPhysicsAngular", ImGuiDataType_Float, &casted->mAngularVelocity.z, 0.01f, 0, 0, 0, flags);
				ImGui::PopItemWidth();
				ImGui::TreePop();
			}
		}
		ImGui::Separator();
	}

	PropertiesPanel::PropertiesPanel() {}

	void PropertiesPanel::OnDraw() {
		ImGui::SetNextWindowSizeConstraints({ 150, 50 }, { FLT_MAX, FLT_MAX });
		ImGui::Begin("Properties");
		if (EditorState::Temp.SimulatingGame) ImGui::BeginDisabled();

		auto& obj = EditorState::Temp.SelectedObject;
		if (!obj) {
			ImGui::Text("No object selected");
		} else {
			ImGui::Text("Instance Name: " + obj->mInstanceName);
			std::string objFullTypeName = typeid(*obj).name();
			auto objTypeName = objFullTypeName.substr(objFullTypeName.find_last_of(":") + 1);
			ImGui::Text("Object Type: " + objTypeName);
			ImGui::Text("UUID: " + obj->GetUUID().AsString());
			ImGui::Separator();

			DrawTransformControls(obj->mTransform);

			ImGui::Separator();

			if (ImGui::CollapsingHeader(std::format("Components ({})", obj->GetComponents()->size()).c_str())) {
				for (auto& comp : *obj->GetComponents()) {
					if (ImGui::TreeNode(comp->GetDisplayName())) {
						DrawComponentProperties(comp);
						ImGui::TreePop();
					}
				}
			}
		}
		ImGui::End();
	}
}
