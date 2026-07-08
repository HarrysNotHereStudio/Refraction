#include <format>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include <Settings.h>
#include <Core/Time.h>
#include <Core/Utilities.h>
#include <Classes/Components/Mesh.h>
#include <Classes/Components/APhysics.h>

#include "AImGuiImpl.h"

std::deque<float> deltaHistory;

namespace Refraction::Editor {
	using Engine::Platform::WindowInputState;

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

	Platform::AImGuiImpl::AImGuiImpl(Common::Ref<Engine::Platform::AWindow> window) : mWindow(window) {
		mDeltaHistory.resize(ImGuiImpl_DeltaHistoryMax);
	}

	void Platform::AImGuiImpl::Draw() {
		BeginDraw();
		ImGui::NewFrame();

		DrawMenu();
		DrawRibbon();
		DrawExplorer();
		DrawAssetDrawer();

		DrawDebugInfoWindow();
		ImGui::Begin("Object Properties");
		DrawSelectedTransformControl();
		DrawSelectedComponentControls();
		ImGui::End();

		ImGui::Render();
		EndDraw();

		if (ImGui::GetIO().WantCaptureMouse && !(mWindow->mInputState == WindowInputState::VIEWPORT && ImGui::GetIO().MouseDown[1])) {
			mWindow->mInputState = WindowInputState::GUI;
		} else if (mWindow->mInputState == WindowInputState::GUI) {
			mWindow->mInputState = WindowInputState::VIEWPORT;
		}
	}

	void Platform::AImGuiImpl::HideMouse() {
		ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
	}

	void Platform::AImGuiImpl::DrawMenu() {
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New")) {
					
				}
				if (ImGui::MenuItem("Quit")) {
					mShouldQuit = true;
					CloseWindow();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
				if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {} // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
				if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
				if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
	}

	void Platform::AImGuiImpl::DrawRibbon() {
	}

	void Platform::AImGuiImpl::DrawExplorer() {
	}

	void Platform::AImGuiImpl::DrawProperties() {

	}

	void Platform::AImGuiImpl::DrawAssetDrawer() {
	}

	void Platform::AImGuiImpl::DrawDebugInfoWindow() {
		mDeltaHistory.push_back((float)Time::RenderDelta * 1000);
		if (mDeltaHistory.size() > ImGuiImpl_DeltaHistoryMax) {
			mDeltaHistory.pop_front();
			mDeltaHistory.shrink_to_fit();
		}
		float values[ImGuiImpl_DeltaHistoryMax] = {};
		float average = 0.0f;
		for (int i = 0; i < mDeltaHistory.size(); i++) {
			values[i] = mDeltaHistory[i];
			average += mDeltaHistory[i];
		}
		average /= mDeltaHistory.size();

		ImGui::Begin("Debug Information");
		ImGui::Text("gurt: yo");
		if (ImGui::TreeNode("Rendering")) {
			ImGui::Checkbox("Wireframe", &Settings::CurrentSettings->Graphics.WireframeEnabled);
			ImGui::PlotLines("FPS", values, ImGuiImpl_DeltaHistoryMax, 0, std::format("Avg {:.3f}ms", average).c_str(), 0, 100.0f, ImVec2(0, 80.0f));
			ImGui::Text(std::format("Elapsed: {:.3f}s", Time::GetSessionSec()).c_str());
			ImGui::Text(std::format("Delta: {:.3f}ms", Time::RenderDelta * 1000).c_str());
			ImGui::Text(std::format("FPS: {}", Utilities::DeltaToRate(Time::RenderDelta, 3)).c_str());
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Input")) {
			std::string inputStateStr;
			switch (mWindow->mInputState) {
			case WindowInputState::NONE: inputStateStr = "None"; break;
			case WindowInputState::GUI: inputStateStr = "GUI"; break;
			case WindowInputState::VIEWPORT: inputStateStr = "Viewport"; break;
			}
			ImGui::Text(std::format("Mouse focus: {}", inputStateStr).c_str());
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera")) {
			auto camera = mWindow->GetCurrentCamera();
			ImGui::Text(std::format("Camera frustum: {}", camera->mFrustum.ToString({ .AsInt = false, .Pretty = false })).c_str());
			ImGui::Text(std::format("Camera speed: {}", Settings::CurrentSettings->Controls.CameraSpeed).c_str());
			ImGui::Text(std::format("Camera grid index: {}", camera->mTransform.mSpatialPosition.GridIndex.ToString({ .AsInt = true, .Pretty = false })).c_str());
			ImGui::Text(std::format("Camera cell position: {}", camera->mTransform.mSpatialPosition.CellPosition.ToString({ .AsInt = false, .Pretty = false })).c_str());
			ImGui::Text(std::format("Camera world position: {}", camera->mTransform.GetWorldPosition().ToString({.AsInt = false, .Pretty = false})).c_str());
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Scene")) {
			ImGui::Text(std::format("Mesh count: {}", Components::Mesh::FrameMeshCount).c_str());
			ImGui::Text(std::format("Vertex count: {}", Components::Mesh::FrameVertexCount).c_str());
			ImGui::TreePop();
		}
		ImGui::End();
	}

	void Platform::AImGuiImpl::DrawSelectedTransformControl() {
		if (!mSelectedObject) {
			float tmpX = 0;
			float tmpY = 0;
			float tmpZ = 0;

			ImGui::Text("Instance name:");
			ImGui::Separator();
			ImGui::BeginDisabled();
			ImGui::Text("Transform");
			ImGui::Text("Position");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##Pos", ImGuiDataType_Float, &tmpX); ImGui::SameLine();
			ImGui::DragScalar("Y##Pos", ImGuiDataType_Float, &tmpY); ImGui::SameLine();
			ImGui::DragScalar("Z##Pos", ImGuiDataType_Float, &tmpZ);
			ImGui::PopItemWidth();
			ImGui::Text("Rotation");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##Rot", ImGuiDataType_Float, &tmpX); ImGui::SameLine();
			ImGui::DragScalar("Y##Rot", ImGuiDataType_Float, &tmpY); ImGui::SameLine();
			ImGui::DragScalar("Z##Rot", ImGuiDataType_Float, &tmpZ);
			ImGui::PopItemWidth();
			ImGui::Text("Scale");
			ImGui::PushItemWidth(100);
			ImGui::DragScalar("X##Scl", ImGuiDataType_Float, &tmpX); ImGui::SameLine();
			ImGui::DragScalar("Y##Scl", ImGuiDataType_Float, &tmpY); ImGui::SameLine();
			ImGui::DragScalar("Z##Scl", ImGuiDataType_Float, &tmpZ);
			ImGui::PopItemWidth();
			ImGui::EndDisabled();
		} else {
			// Handle rotation differently because it's not a Vector3
			auto objectRotation = mSelectedObject->mTransform.mOrientation.ToEulerAngles();
			auto displayedRotation = objectRotation;

			ImGui::Text(std::string("Instance name: " + mSelectedObject->mInstanceName).c_str());
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
		ImGui::Text(std::format("UUID: {}", comp->GetUUID().AsString()).c_str());
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
			ImGui::Text(std::format("Source: {}", asMesh->GetSource().string()).c_str());
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
