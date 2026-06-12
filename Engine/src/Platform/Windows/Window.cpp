#include <deque>
#include <format>


#include <Core/Log.h>
#include <Core/Utilities.h>
#include "Settings.h"
#include <Math/Vector2.h>
#include <Math/Vector3.h>

#include "Window.h"


namespace RPlatform = Refraction::Platform;
namespace RCommon = Refraction::Common;

RPlatform::Window::Window() {}

/*
void framebufferResizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
*/
void GLFWErrorCallback(int code, const char* description) {
	Refraction::RenderLog::Error(std::format("GLFW | CODE: {} | MESSAGE: {}", code, description));
}

void RPlatform::Window::Init() {
	glfwInit();
	glfwSetErrorCallback(GLFWErrorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_DEBUG, true);

	mWindow = glfwCreateWindow(Settings::CurrentSettings->Window.Width, Settings::CurrentSettings->Window.Height, Settings::CurrentSettings->Window.Title, NULL, NULL);
	if (!mWindow) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(mWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize GLAD");
	}
	glViewport(0, 0, Settings::CurrentSettings->Window.Width, Settings::CurrentSettings->Window.Height);
	//glfwSetFramebufferSizeCallback(mWindow, framebufferResizeCallback);
	mImGuiImpl = RCommon::NewURef<ImGuiImpl>();
}

void OnMouseScroll(GLFWwindow* window, double _, double yoffset) {
	auto& cameraSpeed = Settings::CurrentSettings->Controls.CameraSpeed;
	cameraSpeed = std::clamp(yoffset > 0 ? cameraSpeed * 1.1f : cameraSpeed * 0.9f, 0.01f, 5.0f);
}

void RPlatform::Window::InitInput() {
	glfwSetScrollCallback(mWindow, OnMouseScroll);
	mInput.inputEnabled = true;
}

void RPlatform::Window::OnUpdate() {
	mImGuiImpl->GetGuiInputState(&mInputState);

	if (mInputState != mInputStateLast) {
		switch (mInputState) {
			case VIEWPORT:
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				mImGuiImpl->mDebugValues.inputStateStr = "Viewport";
				//Log::Info("Cursor locked for viewport");
				break;
			case GUI:
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				mImGuiImpl->mDebugValues.inputStateStr = "GUI";
				//Log::Info("Cursor free for GUI");
				break;
			case NONE:
			default:
				glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				mImGuiImpl->mDebugValues.inputStateStr = "None";
				//Log::Info("Cursor free from application");
				break;
		}
		mInputStateLast = mInputState;
	}
	if (mInputState == VIEWPORT && mInput.inputEnabled && mInput.mouseRight) {
		mImGuiImpl->HideMouse();
	}

	if (mInput.inputEnabled) {
		using Refraction::Enums::WindowInputState;
		using Refraction::Math::Vector3;
		using Refraction::Math::Vector2;

		// Get inputs
		mInput.keyW = (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS);
		mInput.keyA = (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS);
		mInput.keyS = (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS);
		mInput.keyD = (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS);
		mInput.keyQ = (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS);
		mInput.keyE = (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS);
		mInput.keyEsc = (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS);
		mInput.mouseLeft = (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
		mInput.mouseRight = (glfwGetMouseButton(mWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

		double currentMouseX, currentMouseY;
		glfwGetCursorPos(mWindow, &currentMouseX, &currentMouseY);

		// Process inputs
		mInput.inputFocus = (mInputState == VIEWPORT);

		if (mInput.keyEsc && (mInputState == VIEWPORT)) {
			mInputState = WindowInputState::NONE;
		}
		if (mInput.mouseLeft && (mInputState == NONE)) {
			mInputState = WindowInputState::VIEWPORT;
		}

		// Map WASDQE to Vector3 directional input
		auto dirInput = Refraction::Utilities::BoolToVector3(mInput.keyD, mInput.keyA, mInput.keyE, mInput.keyQ, mInput.keyW, mInput.keyS);

		// Camera movement
		if (mInput.inputFocus && mInput.mouseRight) {
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			auto mouseDelta = Vector2((float)(currentMouseX - mInput.lastMouseX), (float)(mInput.lastMouseY - currentMouseY));
			auto angInput = Vector3(mouseDelta.y, mouseDelta.x, 0.0f);

			mCurrentCamera->Move(dirInput, angInput);
		} else {
			glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		mInput.lastMouseX = currentMouseX;
		mInput.lastMouseY = currentMouseY;
	}
}

void RPlatform::Window::Cleanup() {
	glfwTerminate();
}
