#pragma once

#include <iostream>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Settings.h"

enum WindowInputState {
	VIEWPORT,
	GUI,
	NONE
};

class Camera;

class Window
{
public:
	WindowInputState mInputState = NONE;
	struct {
		float fps;
		float deltaTime;
		float elapsedTime;
	} mDebugValues;
	bool mWireframeToggle = false;

	Window();

	int Init(Settings::Window windowSettings);

	void UpdateLoop();
	void DrawGUI();

	void Cleanup();

	GLFWwindow* GetWindow() { return mWindow; }
	Camera* GetCurrentCamera() { return m_pCurrentCamera; }
	void SetCurrentCamera(Camera* pNewCamera) { m_pCurrentCamera = pNewCamera; }

	bool m_framebufferResized = false;
private:
	WindowInputState mInputStateLast = NONE;
	Camera* m_pCurrentCamera = nullptr;

	GLFWwindow* mWindow = nullptr;
};
