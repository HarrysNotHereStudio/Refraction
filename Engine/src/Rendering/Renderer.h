#pragma once

#include <iostream>
#include <map>
#include <thread>
#include <vector>
#include <chrono>

#include <Core/Common.h>
#include <Math/Rect.h>
#include "Settings.h"
#include <Platform/PlatformAPI.h>

#include <Classes/Assets/Shader.h>
#include <Classes/Objects/SceneRoot.h>
#include "Models/BaseScene.h"
#include "Models/BaseCamera.h"
#include "Buffers/UniformBufferObject.h"
#include "Buffers/GBuffer.h"


namespace Refraction::Engine {
	enum class RendererState {
		NONE,
		INIT,
		RUNNING,
		CLEANUP,
		EXIT
	};

	class Renderer {
	public:
		BaseCamera* mCamera = nullptr;

		Renderer();

		void Init();
		void RenderFrame(Common::Ref<Objects::SceneRoot> scene);

		void SetViewport(Math::Rect rect) { mViewportRect = rect; };

		RendererState GetState() { return mState; }
		Common::Ref<Assets::Texture> GetFinalOutput() const { return mFinalOutput; }
	private:
		void UpdateUniformBuffers();
		void Cleanup();

		// Deferred shading functions
		void DSPassGeometry(Common::Ref<Objects::SceneRoot> scene) const;
		void DSPassLighting(Common::Ref<Objects::SceneRoot> scene) const;
		void DSPassFinal() const;

		RendererState mState = RendererState::NONE;

		UniformBufferObject* mUBO = nullptr;
		Assets::Shader* mGeomPassShader = nullptr;
		Assets::Shader* mLightingPassShader = nullptr;
		GBuffer* mGBuffer = nullptr;
		Math::Rect mViewportRect;
		Math::Rect mViewportRectLast = mViewportRect;
		Common::Ref<Assets::Texture> mFinalOutput;

		BaseScene* mLoadedScene = nullptr;

		bool mShouldRender = true;
		bool mWireframeMode = false;
		double mElapsedRenderTime = 0;
		double mDeltaRenderTime = 0;
		double mElapsedTickTime = 0;
		double mDeltaTickTime = 0;
		std::chrono::steady_clock::time_point mStartRenderTime;
		std::chrono::steady_clock::time_point mStartTickTime;
	};

}
