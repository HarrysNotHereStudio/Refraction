#pragma once

#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include <chrono>
#include <functional>

#include <Core/Common.h>
#include <Core/Log.h>
#include <Core/Utilities.h>
#include <Math/Rect.h>
#include "Settings.h"
#include <Platform/PlatformAPI.h>

#include <EngineClasses/Assets/Shader.h>
#include "Models/BaseScene.h"
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
	Refraction::Platform::Window* GetWindow() { return mWindow; }
	BaseCamera* GetCamera() { return mCamera; }

	void InitWindow();
	void Init();
	void Run();

	void SetResourcePath(std::string path) { Refraction::Constants::ResourcePath = path; };
	void SetViewport(Refraction::Math::Rect rect) { mViewportRect = rect; };
	void SetEditorInterfaceDrawCallback(std::function<void()> callback) { mEditorInterfaceDrawCallback = callback; };
private:
	static Renderer* mInstance;

	Renderer();
	void UpdateUniformBuffers() const;
	void Cleanup();

	// Deferred shading functions
	void DSPassGeometry() const;
	void DSPassLighting() const;
	void DSPassFinal() const;

	RendererState mState = RendererState::NONE;
	Refraction::Platform::Window* mWindow = nullptr;

	UniformBufferObject* mUBO = nullptr;
	Refraction::Assets::Shader* mGeomPassShader = nullptr;
	Refraction::Assets::Shader* mLightingPassShader = nullptr;
	GBuffer* mGBuffer = nullptr;
	Refraction::Math::Rect mViewportRect;
	Refraction::Math::Rect mViewportRectLast = mViewportRect;

	BaseCamera* mCamera = nullptr;
	BaseScene* mLoadedScene = nullptr;

	bool mShouldRender = true;
	bool mWireframeMode = false;
	double mElapsedRenderTime = 0;
	double mDeltaRenderTime = 0;
	std::chrono::steady_clock::time_point mStartRenderTime;
	std::function<void()> mEditorInterfaceDrawCallback;
};
