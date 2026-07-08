#include <string>
#include <format>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_IMPL_OPENGL_DEBUG
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Core/Common.h>

#include "ImGuiImpl.h"

namespace Refraction::Editor::Platform::OpenGL {
	void ImGuiImpl::Init() {
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
		ImGui::GetStyle() = Platform::AImGuiImpl::GetDefaultStyle();

		if (!ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)mWindow->GetNativeWindow(), true)) throw std::runtime_error("Failed to init ImGui for GLFW");
		if (!ImGui_ImplOpenGL3_Init("#version 330")) throw std::runtime_error("Failed to init ImGui for OpenGL");

		Log::Editor.Info("ImGui initialised");
	}

	void ImGuiImpl::BeginDraw() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
	}

	void ImGuiImpl::EndDraw() {
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void Platform::OpenGL::ImGuiImpl::CloseWindow() {
		glfwSetWindowShouldClose((GLFWwindow*)mWindow->GetNativeWindow(), true);
	}
}
