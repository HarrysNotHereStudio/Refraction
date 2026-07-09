#include <format>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include <Settings.h>
#include <Core/Time.h>
#include <Core/Utilities.h>
#include <Classes/Components/Mesh.h>
#include <Classes/Components/APhysics.h>
#include <EditorState.h>
#include <ImGuiExtension.h>
#include <EditorPlatform/ADialogs.h>

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

	Platform::AImGuiImpl::AImGuiImpl(Common::Ref<Engine::Platform::AWindow> window, Common::Ref<Engine::Project> project) : mWindow(window), mProject(project) {
		mDeltaHistory.resize(ImGuiImpl_DeltaHistoryMax);
	}

	void Platform::AImGuiImpl::UpdateInputState() {
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
				if (ImGui::MenuItem("Save")) {
					mProject->Save();
				}
				if (ImGui::MenuItem("New")) {
					mProject->New(Dialogs::SelectFolder("Select Project Folder"));
				}
				if (ImGui::MenuItem("Open")) {
					auto path = Dialogs::SelectFile(REFRACTION_PROJECT_EXTENSION, "Select Project File");
					EditorState::Persistent.RecentProjects.push_back(path);
					mProject->Open(path);
				}
				if (ImGui::BeginMenu("Open Recent")) {
					for (std::filesystem::path& path : EditorState::Persistent.RecentProjects) {
						if (!std::filesystem::exists(path)) continue;
						if (ImGui::MenuItem(path.filename().string().c_str())) {
							mProject->Open(path);
						}
					}
					ImGui::EndMenu();
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
			ImGui::Text(std::format("Elapsed: {:.3f}s", Time::GetSessionSec()));
			ImGui::Text(std::format("Delta: {:.3f}ms", Time::RenderDelta * 1000));
			ImGui::Text(std::format("FPS: {}", Utilities::DeltaToRate(Time::RenderDelta, 3)));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Input")) {
			std::string inputStateStr;
			switch (mWindow->mInputState) {
			case WindowInputState::NONE: inputStateStr = "None"; break;
			case WindowInputState::GUI: inputStateStr = "GUI"; break;
			case WindowInputState::VIEWPORT: inputStateStr = "Viewport"; break;
			}
			ImGui::Text(std::format("Mouse focus: {}", inputStateStr));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Camera")) {
			auto camera = mWindow->GetCurrentCamera();
			ImGui::Text(std::format("Camera frustum: {}", camera->mFrustum.ToString({ .AsInt = false, .Pretty = false })));
			ImGui::Text(std::format("Camera speed: {}", Settings::CurrentSettings->Controls.CameraSpeed));
			ImGui::Text(std::format("Camera grid index: {}", camera->mTransform.mSpatialPosition.GridIndex.ToString({ .AsInt = true, .Pretty = false })));
			ImGui::Text(std::format("Camera cell position: {}", camera->mTransform.mSpatialPosition.CellPosition.ToString({ .AsInt = false, .Pretty = false })));
			ImGui::Text(std::format("Camera world position: {}", camera->mTransform.GetWorldPosition().ToString({.AsInt = false, .Pretty = false})));
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Scene")) {
			ImGui::Text(std::format("Mesh count: {}", Components::Mesh::FrameMeshCount));
			ImGui::Text(std::format("Vertex count: {}", Components::Mesh::FrameVertexCount));
			ImGui::TreePop();
		}
		ImGui::End();
	}
}
