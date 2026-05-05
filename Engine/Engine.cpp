#include <iostream>
#include <map>
#include <thread>
#include <string>

#include <Rendering/Renderer.h>
#include <Rendering/Constants.h>

#include "Engine.h"

Settings* settings = new Settings();

struct sInput {
	bool keyW = false;
	bool keyA = false;
	bool keyS = false;
	bool keyD = false;
	bool keyQ = false;
	bool keyE = false;
	double lastMouseX = settings->window.windowWidth / 2;
	double lastMouseY = settings->window.windowHeight / 2;
};
sInput input;

bool pKeyDebounce = false;
bool inputFocus = false;
bool escapeDown = false;
bool lmbDown = false;

void enableInputProcessing(Renderer* pRenderer) {
	Window* pWindow = pRenderer->GetWindow();
	GLFWwindow* pGLFWWindow = pWindow->GetWindow();
	BaseCamera* pCamera = pRenderer->GetCamera();

	// Don't use the callback, it has this weird lag for some reason
	//glfwSetCursorPosCallback(pGLFWWindow, cursorPositionCallback);

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


void runRenderer(Renderer* pRenderer) {
	try {
		pRenderer->Init(settings);
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		system("pause");
		Renderer::DestroyInstance();
		throw e;
	}

	Renderer::DestroyInstance();
}

// Profoundly janky method of telling the engine where to find resources for now
std::string ResourcePath = "";

int RefractionEngine::Init() {
	std::cout << "Initialising Refraction Engine...\n";

	settings->window.windowWidth = 1920;
	settings->window.windowHeight = 1080;
	settings->graphics.viewportWidth = 1920;
	settings->graphics.viewportHeight = 1080;
	settings->controls.cameraSensitivity = 0.5f;
	settings->controls.cameraSpeed = 0.5f;
	settings->graphics.resourcePath = ResourcePath;

	std::thread renderThread;
	std::thread inputThread;
	try {
		Log::Info("Starting render thread...");
		Renderer* renderer = Renderer::GetInstance();
		renderThread = std::thread(runRenderer, renderer);

		// Wait for renderer to initialise
		while (renderer->GetState() != RendererState::RUNNING) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

		// Allow inputs to be processed
		Log::Info("Starting input thread...");
		inputThread = std::thread(enableInputProcessing, renderer);

		// Wait for renderer to clean up before exiting
		while (renderer->GetState() != RendererState::EXIT) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	} catch (...) {
		std::cout << "Error occurred during engine initialisation, aborting\n" << std::endl;
	}

	renderThread.join();
	inputThread.join();


	std::cout << "\n\n--------------------------------" << std::endl;
	std::cout << "Refraction shut down successfully.\n\n";

	return EXIT_SUCCESS;
}

void RefractionEngine::SetResourcePath(std::string path) {
	ResourcePath = path;
}
