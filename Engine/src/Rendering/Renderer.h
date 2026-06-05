#pragma once

#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include <chrono>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <Core/Constants.h>
#include <Core/Log.h>
#include <Core/Utilities.h>
#include "Settings.h"
#include "ShaderManager.h"
#include <Platform/Windows/Window.h>

#include <EngineClasses/Assets/Shader.h>
#include "Models/BaseScene.h"
#include "Models/BaseModel.h"
#include "Models/InstancedModel.h"
#include "Models/BaseCamera.h"
#include "Buffers/UniformBufferObject.h"
#include "Buffers/GBuffer.h"


enum class RendererState {
	NONE,
	INIT,
	RUNNING,
	CLEANUP,
	EXIT
};

class Renderer {
public:
	static Renderer* GetInstance();
	static void DestroyInstance();

	RendererState GetState() { return mState; }
	Refraction::Platform::Windows::Window* GetWindow() { return mWindow; }
	BaseCamera* GetCamera() { return mCamera; }
	
	int Init();

	void SetResourcePath(std::string path) { Refraction::Constants::ResourcePath = path; };
private:
	Renderer();
	void MainLoop();
	void UpdateUniformBuffers() const;
	void Cleanup();

	// Deferred shading functions
	void DSPassGeometry() const;
	void DSPassLighting() const;
	void DSPassFinal() const;

	static Renderer* mInstance;
	RendererState mState = RendererState::NONE;
	Refraction::Platform::Windows::Window* mWindow = nullptr;

	UniformBufferObject* mUBO = nullptr;
	EngineAssets::Shader* mGeomPassShader = nullptr;
	EngineAssets::Shader* mLightingPassShader = nullptr;
	GBuffer* mGBuffer = nullptr;

	BaseCamera* mCamera = nullptr;
	BaseScene* mLoadedScene = nullptr;

	bool mShouldRender = true;
	bool mWireframeMode = false;
	std::chrono::steady_clock::time_point mStartRenderTime;
	double mElapsedRenderTime = 0;
	double mDeltaRenderTime = 0;
};
