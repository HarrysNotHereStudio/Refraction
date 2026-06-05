#include <string>
#include <deque>
#include <format>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Core/Constants.h>
#include <Settings.h>

#include "ImGuiImpl.h"

namespace Platform = Refraction::Platform::OpenGL;

std::deque<float> deltaHistory = {};
const int deltaHistoryMax = 90;

Platform::ImGuiImpl::ImGuiImpl() {
	deltaHistory.resize(deltaHistoryMax);
}

void Platform::ImGuiImpl::Draw() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	deltaHistory.push_back(mDebugValues.deltaTime * 1000);
	if (deltaHistory.size() > deltaHistoryMax) {
		deltaHistory.pop_front();
		deltaHistory.shrink_to_fit();
	}
	float values[deltaHistoryMax] = {};
	float average = 0.0f;
	for (int i = 0; i < deltaHistory.size(); i++) {
		values[i] = deltaHistory[i];
		average += deltaHistory[i];
	}
	average /= deltaHistory.size();

	ImGui::Begin("new window");
	ImGui::Text("gurt: yo");
	ImGui::PlotLines("FPS", values, deltaHistoryMax, 0, std::format("Avg {:.3f}ms", average).c_str(), 0, 100.0f, ImVec2(0, 80.0f));
	ImGui::Text(std::format("Elapsed: {:.3f}s", mDebugValues.elapsedTime).c_str());
	ImGui::Text(std::format("Delta: {:.3f}ms", mDebugValues.deltaTime * 1000).c_str());
	ImGui::Text(std::format("FPS: {:.0f}", mDebugValues.fps).c_str());
	ImGui::Text(std::format("Mouse focus: {}", mDebugValues.inputStateStr).c_str());
	ImGui::Text(std::format("Camera speed: {}", Settings::CurrentSettings->Controls.CameraSpeed).c_str());
	ImGui::Checkbox("Wireframe", &Settings::CurrentSettings->Graphics.WireframeEnabled);
	ImGui::End();

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Platform::ImGuiImpl::GetGuiInputState(Refraction::Enums::WindowInputState* inputState) {
	// The actual focus signal is handled outside, but here we decide whether to give it to the viewport or the GUI
	using Refraction::Enums::WindowInputState;
	if (ImGui::GetIO().WantCaptureMouse && *inputState != WindowInputState::VIEWPORT) {
		*inputState = WindowInputState::GUI;
	} else if (*inputState == WindowInputState::GUI) {
		*inputState = WindowInputState::NONE;
	}
}

void Platform::ImGuiImpl::HideMouse() {
	ImGui::GetIO().MousePos = ImVec2(-FLT_MAX, -FLT_MAX);
}