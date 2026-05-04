
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Log.h>

#include "Window.h"

Window::Window() {}


void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

int Window::Init(Settings::Window windowSettings) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mWindow = glfwCreateWindow(windowSettings.windowWidth, windowSettings.windowHeight, windowSettings.windowTitle, NULL, NULL);
	if (mWindow == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, windowSettings.windowWidth, windowSettings.windowHeight);
	glfwSetFramebufferSizeCallback(mWindow, framebufferResizeCallback);
	return 0;
}

void Window::UpdateLoop() {
	// We handle input focus outside of this class.
	// Do not allow to regain input focus if the user clicks on GUI while the cursor is free.
	if (ImGui::GetIO().WantCaptureMouse && mInputState != VIEWPORT) {
		mInputState = GUI;
	} else if (mInputState == GUI) {
		mInputState = NONE;
	}

	if (mInputState != mInputStateLast) {
		switch (mInputState) {
			case VIEWPORT:
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				Log::Info("Cursor locked for viewport");
				break;
			case GUI:
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				Log::Info("Cursor free for GUI");
				break;
			case NONE:
			default:
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				Log::Info("Cursor free from application");
				break;
		}
		mInputStateLast = mInputState;
	}
	if (mInputState == VIEWPORT) {
		ImGui::GetIO().MousePos = ImVec2(-FLT_MAX,-FLT_MAX); // Hide mouse from ImGui
	}
	//ImGui::SetNextFrameWantCaptureMouse(mInputState != VIEWPORT);
}

void Window::DrawGUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("new window");
	ImGui::Text("gurt: yo");
	ImGui::End();

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::Cleanup() {
	glfwTerminate();
}
