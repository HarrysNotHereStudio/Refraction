#include <Settings.h>
#include <EngineClasses/Components/APhysics.h>
#include <EngineClasses/Components/Mesh.h>

#include "AImGuiImpl.h"

namespace Refraction {
	ImGuiStyle Platform::AImGuiImpl::GetDefaultStyle() {
		ImGuiStyle style = ImGuiStyle();
		style.WindowPadding = ImVec2(6, 6);
		style.FramePadding = ImVec2(2, 2);
		style.ItemSpacing = ImVec2(8, 2);
		style.ItemInnerSpacing = ImVec2(4, 2);
		style.WindowBorderSize = 1;
		style.ChildBorderSize = 1;
		style.PopupBorderSize = 1;
		style.FrameBorderSize = 0;
		style.WindowRounding = 1;
		style.ChildRounding = 0;
		style.FrameRounding = 0;
		style.PopupRounding = 0;
		style.GrabRounding = 0;
		style.ScrollbarSize = 8;
		style.ScrollbarRounding = 0;
		style.ScrollbarPadding = 2;
		style.TabBorderSize = 0;
		style.TabBarBorderSize = 1;
		style.TabRounding = 0;

		return style;
	}

	Platform::AImGuiImpl::AImGuiImpl() {
		RenderLog::Info("Initialising ImGui...");
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui::GetStyle() = Platform::AImGuiImpl::GetDefaultStyle();

		mDebugValues.deltaHistory.resize(ImGuiImpl_DeltaHistoryMax);
	}

	void Platform::AImGuiImpl::Draw() {
		BeginDraw();
		ImGui::NewFrame();

		DrawDebugInfoWindow();
		ImGui::Begin("Object Properties");
		DrawSelectedTransformControl();
		DrawSelectedComponentControls();
		ImGui::End();

		ImGui::Render();
		EndDraw();
	}

	void Platform::AImGuiImpl::GetGuiInputState(Refraction::Enums::WindowInputState* inputState) {
		// The actual focus signal is handled outside, but here we decide whether to give it to the viewport or the GUI
		using Refraction::Enums::WindowInputState;
		if (ImGui::GetIO().WantCaptureMouse && !(*inputState == WindowInputState::VIEWPORT && ImGui::GetIO().MouseDown[1])) {
			*inputState = WindowInputState::GUI;
		} else if (*inputState == WindowInputState::GUI) {
			*inputState = WindowInputState::VIEWPORT;
		}
	}

	void Platform::AImGuiImpl::HideMouse() {
		ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	}

	void Platform::AImGuiImpl::DrawDebugInfoWindow() {
		mDebugValues.deltaHistory.push_back(mDebugValues.deltaTime * 1000);
		if (mDebugValues.deltaHistory.size() > ImGuiImpl_DeltaHistoryMax) {
			mDebugValues.deltaHistory.pop_front();
			mDebugValues.deltaHistory.shrink_to_fit();
		}
		float values[ImGuiImpl_DeltaHistoryMax] = {};
		float average = 0.0f;
		for (int i = 0; i < mDebugValues.deltaHistory.size(); i++) {
			values[i] = mDebugValues.deltaHistory[i];
			average += mDebugValues.deltaHistory[i];
		}
		average /= mDebugValues.deltaHistory.size();

		ImGui::Begin("Debug Information");
		ImGui::Text("gurt: yo");
		if (ImGui::TreeNode("Rendering")) {
			ImGui::Checkbox("Wireframe", &Settings::CurrentSettings->Graphics.WireframeEnabled);
			ImGui::PlotLines("FPS", values, ImGuiImpl_DeltaHistoryMax, 0, std::format("Avg {:.3f}ms", average).c_str(), 0, 100.0f, ImVec2(0, 80.0f));
			ImGui::Text(std::format("Elapsed: {:.3f}s", mDebugValues.elapsedTime).c_str());
			ImGui::Text(std::format("Delta: {:.3f}ms", mDebugValues.deltaTime * 1000).c_str());
			ImGui::Text(std::format("FPS: {:.0f}", mDebugValues.fps).c_str());
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Input")) {
			ImGui::Text(std::format("Mouse focus: {}", mDebugValues.inputStateStr).c_str());
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera")) {
			ImGui::Text(std::format("Camera speed: {}", Settings::CurrentSettings->Controls.CameraSpeed).c_str());
			ImGui::Text(std::format("Camera grid index: {}", mDebugValues.cameraGridIndex.ToString({ .AsInt = true, .Pretty = false })).c_str());
			ImGui::Text(std::format("Camera cell position: {}", mDebugValues.cameraCellPos.ToString({ .AsInt = false, .Pretty = false })).c_str());
			ImGui::Text(std::format("Camera world position: {}", mDebugValues.cameraWorldPos.ToString({ .AsInt = false, .Pretty = false })).c_str());
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Scene")) {
			ImGui::Text(std::format("Mesh count: {}", mDebugValues.meshCount).c_str());
			ImGui::Text(std::format("Vertex count: {}", mDebugValues.vertexCount).c_str());
			ImGui::TreePop();
		}
		ImGui::End();
	}

	void Platform::AImGuiImpl::DrawSelectedTransformControl() {
		if (!mSelectedObject) {
			float tmpX = 0;
			float tmpY = 0;
			float tmpZ = 0;

			ImGui::Text("No object selected");
			ImGui::Separator();
			ImGui::BeginDisabled();
			ImGui::Text("Transform");
			ImGui::Text("Position");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X", ImGuiDataType_Float, &tmpX); ImGui::SameLine();
			ImGui::DragScalar("Y", ImGuiDataType_Float, &tmpY); ImGui::SameLine();
			ImGui::DragScalar("Z", ImGuiDataType_Float, &tmpZ);
			ImGui::PopItemWidth();
			ImGui::Text("Rotation");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X", ImGuiDataType_Float, &tmpX); ImGui::SameLine();
			ImGui::DragScalar("Y", ImGuiDataType_Float, &tmpY); ImGui::SameLine();
			ImGui::DragScalar("Z", ImGuiDataType_Float, &tmpZ);
			ImGui::PopItemWidth();
			ImGui::Text("Scale");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X", ImGuiDataType_Float, &tmpX); ImGui::SameLine();
			ImGui::DragScalar("Y", ImGuiDataType_Float, &tmpY); ImGui::SameLine();
			ImGui::DragScalar("Z", ImGuiDataType_Float, &tmpZ);
			ImGui::PopItemWidth();
			ImGui::EndDisabled();
		} else {
			// Handle rotation differently because it's not a Vector3
			auto objectRotation = mSelectedObject->mTransform.mOrientation.ToEulerAngles();
			auto displayedRotation = objectRotation;

			ImGui::Text("Transform");
			ImGui::Text("Position");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##Pos", ImGuiDataType_Float, &mSelectedObject->mTransform.mSpatialPosition.CellPosition.x); ImGui::SameLine();
			ImGui::DragScalar("Y##Pos", ImGuiDataType_Float, &mSelectedObject->mTransform.mSpatialPosition.CellPosition.y); ImGui::SameLine();
			ImGui::DragScalar("Z##Pos", ImGuiDataType_Float, &mSelectedObject->mTransform.mSpatialPosition.CellPosition.z);
			ImGui::PopItemWidth();
			ImGui::Text("Rotation");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##Rot", ImGuiDataType_Float, &displayedRotation.x); ImGui::SameLine();
			ImGui::DragScalar("Y##Rot", ImGuiDataType_Float, &displayedRotation.y); ImGui::SameLine();
			ImGui::DragScalar("Z##Rot", ImGuiDataType_Float, &displayedRotation.z);
			ImGui::Text(std::format("Quaternion value: {}", mSelectedObject->mTransform.mOrientation.ToString({ .AsInt = false, .Pretty = false })).c_str());
			ImGui::PopItemWidth();
			ImGui::Text("Scale");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##Scl", ImGuiDataType_Float, &mSelectedObject->mTransform.mScale.x); ImGui::SameLine();
			ImGui::DragScalar("Y##Scl", ImGuiDataType_Float, &mSelectedObject->mTransform.mScale.y); ImGui::SameLine();
			ImGui::DragScalar("Z##Scl", ImGuiDataType_Float, &mSelectedObject->mTransform.mScale.z);
			ImGui::PopItemWidth();

			if (objectRotation != displayedRotation) {
				mSelectedObject->mTransform.mOrientation = Math::Quaternion::FromEulerAngles(displayedRotation);
			}
		}
	}

	void DrawComponentControl(Common::Ref<Components::AComponent>& comp) {
		auto asAPhysics = dynamic_cast<Components::APhysics*>(comp.get());
		ImGui::Text(std::format("Name: {}", comp->GetDisplayName()).c_str());
		ImGui::Text(std::format("UUID: {}", comp->GetUUID()).c_str());
		if (asAPhysics) {
			ImGui::Separator();
			ImGui::Text("Rigid Physics Component");
			ImGui::Text("Linear Velocity");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##RigidPhysicsLinear", ImGuiDataType_Float, &asAPhysics->mLinearVelocity.x); ImGui::SameLine();
			ImGui::DragScalar("Y##RigidPhysicsLinear", ImGuiDataType_Float, &asAPhysics->mLinearVelocity.y); ImGui::SameLine();
			ImGui::DragScalar("Z##RigidPhysicsLinear", ImGuiDataType_Float, &asAPhysics->mLinearVelocity.z);
			ImGui::PopItemWidth();
			ImGui::Text("Angular Velocity");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##RigidPhysicsAngular", ImGuiDataType_Float, &asAPhysics->mAngularVelocity.x); ImGui::SameLine();
			ImGui::DragScalar("Y##RigidPhysicsAngular", ImGuiDataType_Float, &asAPhysics->mAngularVelocity.y); ImGui::SameLine();
			ImGui::DragScalar("Z##RigidPhysicsAngular", ImGuiDataType_Float, &asAPhysics->mAngularVelocity.z);
			ImGui::PopItemWidth();
			return;
		}

		auto asMesh = dynamic_cast<Components::Mesh*>(comp.get());
		if (asMesh) {
			ImGui::Separator();
			ImGui::Text("Mesh Component");
			ImGui::Text(std::format("Source: {}", asMesh->GetSource()).c_str());
			return;
		}
	}

	void Platform::AImGuiImpl::DrawSelectedComponentControls() {
		if (!mSelectedObject) return;
		auto comps = mSelectedObject->GetComponents();
		for (size_t i = 0; i < comps->size(); i++) {
			DrawComponentControl(comps->at(i));
		}
	}
}

