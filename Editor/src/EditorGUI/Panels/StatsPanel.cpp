#include <format>

#include <imgui/imgui.h>

#include <Core/Time.h>
#include <Core/Utilities.h>
#include <Classes/Components/Mesh.h>
#include <Rendering/RenderLayer.h>
#include <EditorState.h>

#include "StatsPanel.h"

constexpr int ImGuiImpl_DeltaHistoryMax = 300;

namespace Refraction::Editor::GUI {
	void StatsPanel::Init() {
		mDeltaHistory.resize(ImGuiImpl_DeltaHistoryMax);
	}

	void StatsPanel::OnDraw() {
		// Don't update FPS graph if nothing is loaded
		auto& project = EditorState::Temp.ProjectInstance;
		if (project->IsLoaded()) {
			mDeltaHistory.push_back((float)Time::RenderDelta * 1000);
			if (mDeltaHistory.size() > ImGuiImpl_DeltaHistoryMax) {
				mDeltaHistory.pop_front();
				mDeltaHistory.shrink_to_fit();
			}
		}

		float values[ImGuiImpl_DeltaHistoryMax] = {};
		float average = 0.0f;
		for (int i = 0; i < mDeltaHistory.size(); i++) {
			values[i] = mDeltaHistory[i];
			average += mDeltaHistory[i];
		}
		average /= mDeltaHistory.size();

		if (!EditorState::Temp.PanelStatisticsVisible) return;
		ImGui::SetNextWindowSizeConstraints({ 150, 50 }, { FLT_MAX, FLT_MAX });
		ImGui::Begin("Stats", &EditorState::Temp.PanelStatisticsVisible);

		auto scene = project->GetActiveScene();
		auto& camera = Objects::Camera::ActiveCamera;
		if (ImGui::TreeNodeEx("Rendering", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::PlotLines("FPS", values, ImGuiImpl_DeltaHistoryMax, 0, std::format("Avg {:.3f}ms", average).c_str(), 0, 100.0f, ImVec2(0, 80.0f));
			ImGui::Text(std::format("Elapsed: {:.3f}s", Time::GetSessionSec()));
			ImGui::Text(std::format("Delta: {:.3f}ms", Time::RenderDelta * 1000));
			ImGui::Text(std::format("FPS: {}", Utilities::DeltaToRate(Time::RenderDelta, 3)));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Input")) {
			std::string inputStateStr;
			switch (mWindow->mInputState) {
			case Engine::Platform::WindowInputState::NONE: inputStateStr = "None"; break;
			case Engine::Platform::WindowInputState::GUI: inputStateStr = "GUI"; break;
			case Engine::Platform::WindowInputState::VIEWPORT: inputStateStr = "Viewport"; break;
			}
			ImGui::Text(std::format("Mouse focus: {}", inputStateStr));
			ImGui::TreePop();
		}
		if (!camera) {
			ImGui::BeginDisabled();
			ImGui::SetNextItemOpen(false);
		}
		if (ImGui::TreeNode("Camera")) {
			ImGui::Text(std::format("Camera frustum: {}", camera->mFrustum.ToString({ .AsInt = false, .Pretty = false })));
			ImGui::Text(std::format("Camera speed: {}", Settings::CurrentSettings->Controls.CameraSpeed));
			ImGui::Text(std::format("Camera grid index: {}", camera->mTransform.mSpatialPosition.GridIndex.ToString({ .AsInt = true, .Pretty = false })));
			ImGui::Text(std::format("Camera cell position: {}", camera->mTransform.mSpatialPosition.CellPosition.ToString({ .AsInt = false, .Pretty = false })));
			ImGui::Text(std::format("Camera world position: {}", camera->mTransform.GetWorldPosition().ToString({ .AsInt = false, .Pretty = false })));
			ImGui::Text(std::format("Camera forward vector: {}", camera->mTransform.GetForwardVector().ToString({ .AsInt = false, .Pretty = false })));
			ImGui::TreePop();
		}
		if (!camera) ImGui::EndDisabled();
		if (scene.expired()) {
			ImGui::BeginDisabled();
			ImGui::SetNextItemOpen(false);
		}
		if (ImGui::TreeNode("Scene")) {
			ImGui::Text(std::format("Mesh count: {}", Components::Mesh::FrameMeshCount));
			ImGui::Text(std::format("Vertex count: {}", Components::Mesh::FrameVertexCount));
			ImGui::TreePop();
		}
		if (scene.expired()) ImGui::EndDisabled();

		ImGui::End();
	}
}
