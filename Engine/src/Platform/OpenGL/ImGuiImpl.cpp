#include <string>
#include <format>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Core/Common.h>

#include "ImGuiImpl.h"

namespace Refraction::Platform::OpenGL {
	ImGuiImpl::ImGuiImpl() {}

	void ImGuiImpl::BeginDraw() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
	}

	void ImGuiImpl::EndDraw() {
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
