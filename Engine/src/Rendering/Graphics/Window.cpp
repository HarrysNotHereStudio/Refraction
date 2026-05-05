#include <deque>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <Log.h>

#include "Window.h"

std::deque<float> deltaHistory = {};
const int deltaHistoryMax = 90;
Window::Window() {
	deltaHistory.resize(deltaHistoryMax);
}


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
	// The actual focus signal is handled outside, but here we decide whether to give it to the viewport or the GUI
	if (ImGui::GetIO().WantCaptureMouse && mInputState != VIEWPORT) {
		mInputState = GUI;
	} else if (mInputState == GUI) {
		mInputState = NONE;
	}

	if (mInputState != mInputStateLast) {
		switch (mInputState) {
			case VIEWPORT:
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				//Log::Info("Cursor locked for viewport");
				break;
			case GUI:
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				//Log::Info("Cursor free for GUI");
				break;
			case NONE:
			default:
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				//Log::Info("Cursor free from application");
				break;
		}
		mInputStateLast = mInputState;
	}
	if (mInputState == VIEWPORT) {
		ImGui::GetIO().MousePos = ImVec2(-FLT_MAX,-FLT_MAX); // Hide mouse from ImGui
	}
}

void Window::DrawGUI() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	std::string inputStateStr = "";
	switch (mInputState) {
		case VIEWPORT:
			inputStateStr = "Viewport";
			break;
		case GUI:
			inputStateStr = "GUI";
			break;
		case NONE:
			inputStateStr = "None";
			break;
	}

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
	ImGui::PlotLines("Lines", values, deltaHistoryMax, 0, std::format("Avg {:.3f}ms", average).c_str(), 0, 100.0f, ImVec2(0, 80.0f));
	ImGui::Text(std::format("Elapsed: {:.3f}s", mDebugValues.elapsedTime).c_str());
	ImGui::Text(std::format("Delta: {:.3f}ms", mDebugValues.deltaTime*1000).c_str());
	ImGui::Text(std::format("FPS: {:.0f}", mDebugValues.fps).c_str());
	ImGui::Text(std::format("Mouse focus: {}", inputStateStr).c_str());
	ImGui::Checkbox("Wireframe", &mWireframeToggle);
	ImGui::End();

	ImGui::ShowDemoWindow();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Window::Cleanup() {
	glfwTerminate();
}
