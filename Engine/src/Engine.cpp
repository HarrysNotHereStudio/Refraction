#include <iostream>
#include <map>
#include <thread>
#include <string>

#include <EngineConstants.h>
#include <EngineLog.h>
#include <EngineUtilities.h>
#include <Rendering/Renderer.h>

#include "Engine.h"

Settings* Settings::CurrentSettings = new Settings();

namespace {
	struct sInput {
		double lastMouseX = Settings::CurrentSettings->Window.Width / 2.0;
		double lastMouseY = Settings::CurrentSettings->Window.Height / 2.0;
		bool keyW = false;
		bool keyA = false;
		bool keyS = false;
		bool keyD = false;
		bool keyQ = false;
		bool keyE = false;
	};
	sInput input;

	bool pKeyDebounce = false;
	bool inputFocus = false;
	bool escapeDown = false;
	bool lmbDown = false;

	void OnMouseScroll(GLFWwindow* window, double _, double yoffset) {
		auto& cameraSpeed = Settings::CurrentSettings->Controls.CameraSpeed;
		cameraSpeed = std::clamp(yoffset > 0 ? cameraSpeed * 1.1f : cameraSpeed * 0.9f, 0.1f, 2.0f);
	}

	void EnableInputProcessing(Renderer* pRenderer) {
		Window* pWindow = pRenderer->GetWindow();
		GLFWwindow* pGLFWWindow = pWindow->GetWindow();
		BaseCamera* pCamera = pRenderer->GetCamera();

		// Don't use the callback, it has this weird lag for some reason
		//glfwSetCursorPosCallback(pGLFWWindow, cursorPositionCallback);
		glfwSetScrollCallback(pGLFWWindow, OnMouseScroll);

		while (pRenderer->GetState() == RendererState::RUNNING) {

			// Get inputs
			input.keyW = (glfwGetKey(pGLFWWindow, GLFW_KEY_W) == GLFW_PRESS);
			input.keyA = (glfwGetKey(pGLFWWindow, GLFW_KEY_A) == GLFW_PRESS);
			input.keyS = (glfwGetKey(pGLFWWindow, GLFW_KEY_S) == GLFW_PRESS);
			input.keyD = (glfwGetKey(pGLFWWindow, GLFW_KEY_D) == GLFW_PRESS);
			input.keyQ = (glfwGetKey(pGLFWWindow, GLFW_KEY_Q) == GLFW_PRESS);
			input.keyE = (glfwGetKey(pGLFWWindow, GLFW_KEY_E) == GLFW_PRESS);
			escapeDown = (glfwGetKey(pGLFWWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS);
			lmbDown = (glfwGetMouseButton(pGLFWWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

			double currentMouseX, currentMouseY;
			glfwGetCursorPos(pGLFWWindow, &currentMouseX, &currentMouseY);

			// Process inputs
			// fyi the actual cursor focusing code is in the Window class now because it has to be in the same thread as whatever handles the window
			inputFocus = pWindow->mInputState == VIEWPORT;

			if (escapeDown && pWindow->mInputState == VIEWPORT) {
				pWindow->mInputState = NONE;
			}
			if (lmbDown && pWindow->mInputState == NONE) {
				pWindow->mInputState = VIEWPORT;
				input.lastMouseX = currentMouseX;
				input.lastMouseY = currentMouseY;
			}

			// Map WASDQE to Vector3 directional input
			glm::vec3 dirInput = glm::vec3(0.0f);
			if (input.keyW) dirInput.z = 1;
			if (input.keyA) dirInput.x = -1;
			if (input.keyS) dirInput.z = -1;
			if (input.keyD) dirInput.x = 1;
			if (input.keyQ) dirInput.y = -1;
			if (input.keyE) dirInput.y = 1;

			// Camera rotation
			if (inputFocus) {
				glm::vec2 mouseDelta = glm::vec2(currentMouseX - input.lastMouseX, input.lastMouseY - currentMouseY);
				glm::vec3 angInput = glm::vec3(mouseDelta.y, mouseDelta.x, 0.0f);
				input.lastMouseX = currentMouseX;
				input.lastMouseY = currentMouseY;

				pCamera->Move(dirInput, angInput);
			}

			// Sleep for a bit to reduce CPU usage
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}


	void RunRenderer(Renderer* pRenderer) {
		try {
			Log::Info("Resource path: " + EngineConstants::GetResourcePath());
			pRenderer->SetResourcePath(EngineConstants::GetResourcePath());
			pRenderer->Init();
		} catch (const std::exception& e) {
			std::cerr << e.what() << '\n';
			Renderer::DestroyInstance();
			throw;
		}

		Renderer::DestroyInstance();
	}
}


int RefractionEngine::Init() {
	std::cout << "Initialising Refraction Engine...\n";

	Settings::CurrentSettings->Window.Width = 1920;
	Settings::CurrentSettings->Window.Height = 1080;
	Settings::CurrentSettings->Graphics.ViewportWidth = 1920;
	Settings::CurrentSettings->Graphics.ViewportHeight = 1080;
	Settings::CurrentSettings->Controls.CameraSensitivity = 0.5f;
	Settings::CurrentSettings->Controls.CameraSpeed = 0.5f;

	std::thread renderThread;
	std::thread inputThread;
	try {
		Log::Info("Resource path: " + EngineConstants::GetResourcePath());
		Log::Info("Starting render thread...");
		Renderer* renderer = Renderer::GetInstance();
		renderThread = std::thread(RunRenderer, renderer);

		// Wait for renderer to initialise
		while (renderer->GetState() != RendererState::RUNNING) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		// Allow inputs to be processed
		Log::Info("Starting input thread...");
		inputThread = std::thread(EnableInputProcessing, renderer);

		// Wait for renderer to clean up before exiting
		while (renderer->GetState() != RendererState::EXIT) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	} catch (...) {
		std::cout << "Error occurred during engine initialisation, aborting\n\n";
	}

	renderThread.join();
	inputThread.join();


	std::cout << "\n\n--------------------------------\n";
	std::cout << "Refraction shut down successfully.\n\n";

	Log::Info("hi im a regular print message");
	Log::Warn("hi im a warning message");
	Log::Error("hi im an error message");

	return EXIT_SUCCESS;
}

void RefractionEngine::SetResourcePath(std::string path) {
	EngineConstants::ResourcePath = path;
	Log::Info("Set Resource path to " + EngineConstants::GetResourcePath());
}
