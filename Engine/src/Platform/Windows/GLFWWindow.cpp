#include <deque>
#include <format>

#include <Core/Utilities.h>
#include "Settings.h"
#include <Math/Vector2.h>
#include <Math/Vector3.h>

#include "GLFWWindow.h"


namespace RPlatform = Refraction::Engine::Platform;
namespace RCommon = Refraction::Common;

namespace Refraction::Engine::Platform {
	GLFWWindow::GLFWWindow() {}

	static void GLFWErrorCallback(int code, const char* description) {
		Log::Render.Error(std::format("GLFW | CODE: {} | MESSAGE: {}", code, description));
	}

	void GLFWWindow::Init() {
		glfwInit();
		glfwSetErrorCallback(GLFWErrorCallback);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_DEBUG, true);

		mHandle = glfwCreateWindow(Settings::CurrentSettings->Window.Width, Settings::CurrentSettings->Window.Height, Settings::CurrentSettings->Window.Title, NULL, NULL);
		if (!mHandle) {
			glfwTerminate();
			throw std::runtime_error("Failed to create GLFW window");
		}
		glfwMakeContextCurrent(mHandle);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw std::runtime_error("Failed to initialize GLAD");
		}
		glViewport(0, 0, Settings::CurrentSettings->Window.Width, Settings::CurrentSettings->Window.Height);
	}

	void OnMouseScroll(GLFWwindow* window, double _, double yoffset) {
		auto& cameraSpeed = Settings::CurrentSettings->Controls.CameraSpeed;
		cameraSpeed = std::clamp(yoffset > 0 ? cameraSpeed * 1.1f : cameraSpeed * 0.9f, 0.01f, 5.0f);
	}

	void GLFWWindow::InitInput() {
		glfwSetScrollCallback(mHandle, OnMouseScroll);
		mInput.inputEnabled = true;
	}

	void GLFWWindow::OnUpdate() {
		if (mInput.inputEnabled) {
			using Math::Vector3;
			using Math::Vector2;

			// Get inputs
			mInput.keyW = (glfwGetKey(mHandle, GLFW_KEY_W) == GLFW_PRESS);
			mInput.keyA = (glfwGetKey(mHandle, GLFW_KEY_A) == GLFW_PRESS);
			mInput.keyS = (glfwGetKey(mHandle, GLFW_KEY_S) == GLFW_PRESS);
			mInput.keyD = (glfwGetKey(mHandle, GLFW_KEY_D) == GLFW_PRESS);
			mInput.keyQ = (glfwGetKey(mHandle, GLFW_KEY_Q) == GLFW_PRESS);
			mInput.keyE = (glfwGetKey(mHandle, GLFW_KEY_E) == GLFW_PRESS);
			mInput.keyEsc = (glfwGetKey(mHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS);
			mInput.mouseLeft = (glfwGetMouseButton(mHandle, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
			mInput.mouseRight = (glfwGetMouseButton(mHandle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

			double currentMouseX, currentMouseY;
			glfwGetCursorPos(mHandle, &currentMouseX, &currentMouseY);

			// Process inputs
			mInput.inputFocus = (mInputState == WindowInputState::VIEWPORT);

			if (mInput.keyEsc && (mInputState == WindowInputState::VIEWPORT)) {
				mInputState = WindowInputState::NONE;
			}
			if (mInput.mouseLeft && (mInputState == WindowInputState::NONE)) {
				mInputState = WindowInputState::VIEWPORT;
			}

			// Map WASDQE to Vector3 directional input
			auto dirInput = Utilities::BoolToVector3(mInput.keyD, mInput.keyA, mInput.keyE, mInput.keyQ, mInput.keyW, mInput.keyS);

			// Camera movement
			if (mInput.inputFocus && mInput.mouseRight) {
				glfwSetInputMode(mHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				auto mouseDelta = Vector2((float)(currentMouseX - mInput.lastMouseX), (float)(mInput.lastMouseY - currentMouseY));
				auto angInput = Vector3(mouseDelta.y, mouseDelta.x, 0.0f);

				GetCurrentCamera()->Move(dirInput, angInput);
			} else {
				glfwSetInputMode(mHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			mInput.lastMouseX = currentMouseX;
			mInput.lastMouseY = currentMouseY;
		}

		glfwPollEvents();
		glfwSwapBuffers(mHandle);

		Math::Rect currentRect;
		glfwGetWindowPos(mHandle, &currentRect.x, &currentRect.y);
		glfwGetFramebufferSize(mHandle, &currentRect.w, &currentRect.h);
		if (currentRect != mRect) {
			mRect = currentRect;
			mShouldFramebufferRegen = true;
		}
	}

	void GLFWWindow::Cleanup() {
		glfwTerminate();
	}

	bool GLFWWindow::ShouldClose() const {
		return glfwWindowShouldClose(mHandle);
	}

	void GLFWWindow::SetRect(Math::Rect newRect) {
		glfwSetWindowPos(mHandle, newRect.x, newRect.y);
		glfwSetWindowSize(mHandle, newRect.w, newRect.h);
	}
}
