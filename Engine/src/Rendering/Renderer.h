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
#include <Platform/AGBuffer.h>

#include <Classes/Assets/Shader.h>
#include <Classes/Objects/SceneRoot.h>
#include <Interface/Project.h>
#include "Models/BaseScene.h"
#include "Buffers/UniformBufferObject.h"


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
		Renderer();

		void Init();
		void RenderFrame(Common::Shared<Project> projectInstance);

		void SetViewport(Math::Rect rect) { mViewportRect = rect; };

		RendererState GetState() { return mState; }
		Common::Shared<Assets::Image> GetFinalOutput() const { return mFinalOutput; }
	private:
		void UpdateUniformBuffers(Common::Shared<Project> projectInstance);
		void Cleanup();

		// Deferred shading functions
		void DSPassGeometry(Common::Shared<Project> projectInstance);
		void DSPassLighting(Common::Shared<Project> projectInstance);
		void DSPassFinal() const;

		RendererState mState = RendererState::NONE;

		UniformBufferObject* mUBO = nullptr;
		Common::Shared<Assets::Shader> mGeomPassShader = nullptr;
		Common::Shared<Assets::Shader> mLightingPassShader = nullptr;
		Common::Shared<Platform::AGBuffer> mGBuffer = nullptr;
		Math::Rect mViewportRect;
		Math::Rect mViewportRectLast = mViewportRect;
		Common::Shared<Assets::Image> mFinalOutput;

		BaseScene* mLoadedScene = nullptr;

		bool mShouldRender = true;
		bool mWireframeMode = false;
		bool mCFAALastState = false;
		double mElapsedRenderTime = 0;
		double mDeltaRenderTime = 0;
		double mElapsedTickTime = 0;
		double mDeltaTickTime = 0;
		std::chrono::steady_clock::time_point mStartRenderTime;
		std::chrono::steady_clock::time_point mStartTickTime;
	};

}
