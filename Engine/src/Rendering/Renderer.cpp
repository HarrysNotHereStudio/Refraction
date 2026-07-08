#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <Core/Time.h>
#include <Core/Utilities.h>
#include <Classes/Components/Mesh.h>
#include "Models/Billboard.h"
#include "Models/BaseLight.h"
#include "Models/PointLight.h"

#include "Renderer.h"

namespace Refraction::Engine {
	Renderer::Renderer() = default;

	std::vector<unsigned int> VAOs = {};
	std::vector<unsigned int> VBOs = {};

	Math::Matrix4 projectionMatrix;
	std::chrono::steady_clock::time_point timeRenderLast;
	std::chrono::steady_clock::time_point timeTickLast;

	void Renderer::Init() {
		mState = RendererState::INIT;
		Log::Render.Info("Initializing...");

		mViewportRect = Math::Rect(Settings::CurrentSettings->Window.Width, Settings::CurrentSettings->Window.Height);
		mViewportRectLast = mViewportRect;

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		auto result = glfwGetCurrentContext();

		mCamera->mFrustum.w = mViewportRect.w;
		mCamera->mFrustum.h = mViewportRect.h;

		Log::Render.Info("Loading shaders...");
		Assets::Shader::LoadAllShaders();

		mGeomPassShader = Assets::Shader::GetShaderByName("gbufferShader");
		mLightingPassShader = Assets::Shader::GetShaderByName("lightingShader");

		mLightingPassShader->Activate();
		mLightingPassShader->SetUniformInt("gPosition", 0);
		mLightingPassShader->SetUniformInt("gNormal", 1);
		mLightingPassShader->SetUniformInt("gAlbedoSpec", 2);

		Log::Render.Info("Creating G-Buffer...");
		mGBuffer = new GBuffer();
		if (!mGBuffer->Init(mViewportRect.w, mViewportRect.h)) throw;
		mFinalOutput = Common::NewRef<Assets::Texture>(mGBuffer->GetFinalTextureID());

		Log::Render.Info("Creating uniform buffer object...");
		projectionMatrix = Math::Matrix4::Perspective(mCamera->mFrustum);
		sUBO initData = {
			Utilities::NativeToGLMMat4(mCamera->GetViewMatrix()),
			Utilities::NativeToGLMMat4(projectionMatrix)
		};
		mUBO = new UniformBufferObject(initData);

		Log::Render.Info("Loading test scene...");
		mLoadedScene = new BaseScene();

		mStartTickTime = std::chrono::steady_clock::now();
		mStartRenderTime = std::chrono::steady_clock::now();
		timeTickLast = mStartTickTime;
		timeRenderLast = mStartRenderTime;
		Time::RenderDelta = 0;

		Log::Render.Info("Initialisation complete");
		mState = RendererState::RUNNING;

		return;
	}

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void renderQuad() {
		if (quadVAO == 0) {
			float quadVertices[] = {
				// positions        // texture Coords
				-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			};
			// setup plane VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);
			glBindVertexArray(quadVAO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)nullptr);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		}
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
	}

	void Renderer::RenderFrame(Common::Ref<Objects::SceneRoot> scene) {
		auto timeNow = std::chrono::steady_clock::now();
		mDeltaRenderTime = std::chrono::duration<double>(timeNow - timeRenderLast).count();
		Time::RenderDelta = mDeltaRenderTime;
		mElapsedRenderTime = std::chrono::duration<double>(timeNow - mStartRenderTime).count();
		timeRenderLast = timeNow;

		Components::Mesh::FrameMeshCount = 0;
		Components::Mesh::FrameVertexCount = 0;

		UpdateUniformBuffers();

		// Draw scene
		DSPassGeometry(scene);
		DSPassLighting(scene);
		DSPassFinal();
	}

	void Renderer::UpdateUniformBuffers() {
		sUBO newData{};
		newData.viewMatrix = Utilities::NativeToGLMMat4(mCamera->GetViewMatrix());

		if (mViewportRectLast != mViewportRect) {
			Log::Render.Info("Resizing viewport to " + mViewportRect.ToString({ .Pretty = false }));
			if (!mGBuffer->Regenerate(mViewportRect.w, mViewportRect.h)) throw;
			glViewport(mViewportRect.x, mViewportRect.y, mViewportRect.w, mViewportRect.h);
			mViewportRectLast = mViewportRect;
			mCamera->mFrustum.w = mViewportRect.w;
			mCamera->mFrustum.h = mViewportRect.h;
			projectionMatrix = Math::Matrix4::Perspective(mCamera->mFrustum);
		}
		newData.perspectiveMatrix = Utilities::NativeToGLMMat4(projectionMatrix);
		mUBO->UploadNewData(newData);
	}

	void Renderer::Cleanup() {
		mState = RendererState::CLEANUP;

		Log::Render.Info("Cleaning up...");

		glfwTerminate();
	}


	// Deferred Shading

	void Renderer::DSPassGeometry(Common::Ref<Objects::SceneRoot> scene) const {
		mGBuffer->BindFull();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mGeomPassShader->Activate();

		if (Settings::CurrentSettings->Graphics.WireframeEnabled) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		// Draw models
		scene->RenderScene();

		// Draw billboards
		Billboard::DrawAll();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (Settings::CurrentSettings->Graphics.WireframeEnabled) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void Renderer::DSPassLighting(Common::Ref<Objects::SceneRoot> scene) const {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mGBuffer->BindForLighting();

		mLightingPassShader->Activate();
		for (auto i = 0; i < mLoadedScene->mLights.size(); i++) {
			const auto light = mLoadedScene->mLights[i];
			light->UpdateShaderUniforms(i);
		}
		mLightingPassShader->SetUniformVec3("viewPos", mCamera->mTransform.GetWorldPosition());
		// finally render quad
		renderQuad();
	}

	void Renderer::DSPassFinal() const {
		mGBuffer->BindForFinal();

		int viewX0 = mViewportRect.x;
		int viewY0 = mViewportRect.y;
		int viewX1 = viewX0 + mViewportRect.w;
		int viewY1 = viewY0 + mViewportRect.h;
		glBlitFramebuffer(0, 0, mViewportRect.w, mViewportRect.h, viewX0, viewY0, viewX1, viewY1, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
