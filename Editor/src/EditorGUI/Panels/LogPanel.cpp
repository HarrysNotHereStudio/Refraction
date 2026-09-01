#include <format>

#include <imgui/imgui.h>

#include <EditorState.h>

#include "LogPanel.h"

namespace Refraction::Editor::GUI {
	LogPanel::LogPanel(Common::Shared<Events::AEventDispatcher> eventDispatcher, Common::Shared<Engine::Platform::AWindow> window) : AEditorPanel(eventDispatcher, window) {
		Log::AddLogCallback([&](Log::Colour clr, std::string text, bool newLine) {
			mHistory.push_back(std::make_tuple(clr, text, newLine));
		});
	}

	void LogPanel::OnDraw() {
		if (!EditorState::Temp.PanelLogVisible) return;
		ImGui::SetNextWindowSizeConstraints({ 300, 50 }, { FLT_MAX, FLT_MAX });
		ImGui::Begin("Log", &EditorState::Temp.PanelLogVisible);

		ImGui::Checkbox("Autoscroll", &mShouldAutoscroll);
		ImGui::Separator();

		ImGui::BeginChild("MessageList");
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));
		for (auto& msg : mHistory) {
			auto col = std::get<0>(msg);
			if (!std::get<2>(msg)) ImGui::SameLine();
			ImGui::TextColored(ImVec4((float)col.R / 255, (float)col.G / 255, (float)col.B / 255, 1.0f), std::get<1>(msg).c_str());
		}
		ImGui::PopStyleVar();
		if (mShouldAutoscroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
		ImGui::EndChild();

		ImGui::End();
	}
}
