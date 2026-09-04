#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <Core/Time.h>
#include <Core/Utilities.h>
#include <Classes/Components/Mesh.h>
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
	Math::Frustum defaultProjection = Math::Frustum(1, 1, 90.0f, 0.1f, 1000.0f);
	Math::Transform defaultView = Math::Transform();

	void Renderer::Init() {
		mState = RendererState::INIT;
		Log::Render.Info("Initializing...");

		mViewportRect = Math::Rect(Settings::CurrentSettings->Window.Width, Settings::CurrentSettings->Window.Height);
		mViewportRectLast = mViewportRect;

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		auto result = glfwGetCurrentContext();

		Log::Render.Info("Creating G-Buffer...");
		mGBuffer = Platform::AGBuffer::CreateGBuffer();
		if (!mGBuffer->Init(mViewportRect.w, mViewportRect.h)) throw;
		mFinalOutput = mGBuffer->GetLastRenderedFrame();

		Log::Render.Info("Creating uniform buffer object...");
		projectionMatrix = Math::Matrix4::Perspective(defaultProjection);
		sUBO initData = {
			Utilities::NativeToGLMMat4(defaultView.ToMatrix()),
			Utilities::NativeToGLMMat4(projectionMatrix)
		};
		mUBO = new UniformBufferObject(initData);

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
	static void renderQuad() {
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

	void Renderer::RenderFrame(Common::Shared<Project> projectInstance) {
		auto timeNow = std::chrono::steady_clock::now();
		mDeltaRenderTime = std::chrono::duration<double>(timeNow - timeRenderLast).count();
		Time::RenderDelta = mDeltaRenderTime;
		mElapsedRenderTime = std::chrono::duration<double>(timeNow - mStartRenderTime).count();
		timeRenderLast = timeNow;

		Components::Mesh::FrameMeshCount = 0;
		Components::Mesh::FrameVertexCount = 0;

		// Skip if no project or scene loaded (projects may load asynchronously so this prevents issues) or no active camera
		if (!projectInstance->IsLoaded()) return;
		if (projectInstance->GetActiveScene().expired()) return;
		if (!Objects::Camera::ActiveCamera) return;


		AssetManager::Try([&](Common::Shared<AssetManager> assetManager) {
			auto& graphicsSettings = Settings::CurrentSettings->Graphics;
			if (mGeomPassShader.expired()) {
				mGeomPassShader = assetManager->GetAsset<Assets::Shader>("gbufferShader");
			}
			if (mLightingPassShader.expired()) {
				mLightingPassShader = assetManager->GetAsset<Assets::Shader>("lightingShader");
				auto shader = mLightingPassShader.lock();
				shader->Activate();
				shader->SetUniformVec3("ambient", Math::Vector3(0.2f));
				mGBuffer->SetShaderTextureIDs();
			}
			if (graphicsSettings.CFAAEnabled && mCFAAPrepassShader.expired()) {
				mCFAAPrepassShader = assetManager->GetAsset<Assets::Shader>("CFAAPrepass");
			}
			if (mSkyShader.expired()) {
				mSkyShader = assetManager->GetAsset<Assets::Shader>("DefaultSky");
			}

			if (!mLoadedScene) {
				Log::Render.Info("Loading test scene...");
				mLoadedScene = new BaseScene();
			}
		});

		UpdateUniformBuffers(projectInstance);

		mGBuffer->StartFrame();

		// Draw scene
		DSPassGeometry(projectInstance);
		DSPassLighting(projectInstance);
		DSPassFinal();
	}

	void Renderer::UpdateUniformBuffers(Common::Shared<Project> projectInstance) {
		auto& camera = Objects::Camera::ActiveCamera;
		if (!camera) return;
		sUBO newData{};
		newData.viewMatrix = Utilities::NativeToGLMMat4(camera->GetViewMatrix());

		if (mViewportRectLast != mViewportRect) {
			if (!mGBuffer->Regenerate(mViewportRect.w, mViewportRect.h)) throw;
			glViewport(0, 0, mViewportRect.w, mViewportRect.h);
			camera->mFrustum.w = mViewportRect.w;
			camera->mFrustum.h = mViewportRect.h;
			projectionMatrix = Math::Matrix4::Perspective(camera->mFrustum);
			mViewportRectLast = mViewportRect;
		}
		auto& cfaaEnabled = Settings::CurrentSettings->Graphics.CFAAEnabled;
		if (mCFAALastState != cfaaEnabled) {
			mCFAALastState = cfaaEnabled;
			// Regenerate GBuffer to rescale GBuffer frames
			if (!mGBuffer->Regenerate(mViewportRect.w, mViewportRect.h)) throw;
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

	void Renderer::DSPassGeometry(Common::Shared<Project> projectInstance) {
		auto& graphicsSettings = Settings::CurrentSettings->Graphics;
		mGBuffer->BindGeometryPass();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (graphicsSettings.WireframeEnabled) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

		auto scene = projectInstance->GetActiveScene().lock();

		if (graphicsSettings.CFAAEnabled) {
			// CFAA prepass
			auto shader = mCFAAPrepassShader.lock();
			shader->Activate();
			scene->RenderScene(projectInstance->GetGlobalObjects());
		}

		// Draw models
		auto shader = mGeomPassShader.lock();
		shader->Activate();
		shader->SetUniformBool("usingCFAA", graphicsSettings.CFAAEnabled);
		shader->SetUniformInt("CFAAScale", graphicsSettings.CFAAScale);
		scene->RenderScene(projectInstance->GetGlobalObjects());

		if (graphicsSettings.WireframeEnabled) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	void Renderer::DSPassLighting(Common::Shared<Project> projectInstance) {
		auto& graphicsSettings = Settings::CurrentSettings->Graphics;
		mGBuffer->BindLightingPass();

		auto lightShader = mLightingPassShader.lock();

		lightShader->Activate();
		for (auto i = 0; i < mLoadedScene->mLights.size(); i++) {
			const auto light = mLoadedScene->mLights[i];
			light->UpdateShaderUniforms(i);
		}
		lightShader->SetUniformVec3("viewPos", Objects::Camera::ActiveCamera->mTransform.GetWorldPosition());
		lightShader->SetUniformInt("dataView", graphicsSettings.ViewportDataView);
		lightShader->SetUniformBool("usingCFAA", graphicsSettings.CFAAEnabled);
		lightShader->SetUniformInt("CFAAScale", graphicsSettings.CFAAScale);

		glDepthMask(GL_FALSE);
		// Render sky
		auto skyShader = mSkyShader.lock();
		skyShader->Activate();
		renderQuad();
		// Render grid
		//Assets::Shader::GetShaderByName("EditorGrid")->Activate();
		//renderQuad();
		glDepthMask(GL_TRUE);

		// Render lit objects
		lightShader->Activate();
		renderQuad();
	}

	void Renderer::DSPassFinal() const {
		mGBuffer->BindFinalPass();

		glBlitFramebuffer(0, 0, mViewportRect.w, mViewportRect.h, 0, 0, mViewportRect.w, mViewportRect.h, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}
