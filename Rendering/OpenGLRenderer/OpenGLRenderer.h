#pragma once

#include <iostream>
#include <map>
#include <thread>
#include <vector>

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "Log.h"
#include "Utilities.h"
#include "Settings.h"
#include "Graphics/Window.h"
#include "Graphics/BaseShader.h"
#include "Models/BaseModel.h"
#include "Models/BaseCamera.h"
#include "Models/BaseLight.h"
#include "Models/PointLight.h"
#include "Graphics/Buffers/UniformBufferObject.h"
#include "Graphics/Buffers/GBuffer.h"


enum class OpenGLRendererState
{
	NONE,
	INIT,
	RUNNING,
	CLEANUP,
	EXIT
};

struct sSettings {
	struct sControlSettings {
		float cameraSensitivity = .1f; // Sensitivity of the camera movement.
		float cameraSpeed = 0.05f; // Speed of the camera movement.
	} control;
};


class OpenGLRenderer
{
public:
	static OpenGLRenderer* getInstance();
	static void destroyInstance();
	OpenGLRendererState getState() { return m_state; }
	Window* getWindow() { return m_pWindow; }
	BaseCamera* getCamera() { return m_pCamera; }
	int init(Settings* initSettings);
private:
	OpenGLRenderer();
	void loadShaders();
	void mainLoop();
	void updateUniformBuffers();
	void cleanup();

	// Deferred shading functions
	void DSPassGeometry();
	void DSPassStencil(PointLight* pPointLight);
	void DSPassPointLight(PointLight* pPointLight);
	void DSPassDirectionalLight();
	void DSPassFinal();

	static OpenGLRenderer* m_pInstance;
	OpenGLRendererState m_state = OpenGLRendererState::NONE;
	Settings m_currentSettings;
	Window* m_pWindow = nullptr;

	UniformBufferObject* m_pUBO = nullptr;
	BaseShader* m_pLightingPassShader = nullptr;
	GBuffer* m_pGBuffer = nullptr;

	BaseCamera* m_pCamera = nullptr;
	vector<PointLight*> m_pScenePointLights = {};

	bool m_shouldRender = true;
	double m_startRenderTime = 0.0f;
	double m_elapsedRenderTime = 0.0f;
	double m_deltaRenderTime = 0.0f;
};

inline void nativeDebugPrint(std::string message, bool newLine = false) { std::cout << (newLine ? "\n" : "") << "OpenGLRenderer DEBUG - " << message << std::endl; }