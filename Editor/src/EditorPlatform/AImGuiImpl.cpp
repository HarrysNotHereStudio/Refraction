#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>

#include <Settings.h>

#include "AImGuiImpl.h"

namespace RefractionEditor {
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
	}

    void Platform::AImGuiImpl::Draw() {
        BeginDraw();
        ImGui::NewFrame();

        DrawMenu();
        DrawRibbon();
        DrawExplorer();
        DrawAssetDrawer();

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

	void Platform::AImGuiImpl::DrawMenu() {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
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

    void Platform::AImGuiImpl::DrawAssetDrawer() {
    }
}
