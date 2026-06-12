#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <EngineClasses/Components/Mesh.h>
#include "Models/Billboard.h"
#include "Models/BaseLight.h"
#include "Models/PointLight.h"

#include "Renderer.h"

using namespace Refraction;

Renderer* Renderer::mInstance = nullptr;

Renderer::Renderer() = default;

Renderer* Renderer::GetInstance() {
	if (mInstance == nullptr) {
		mInstance = new Renderer();
		RenderLog::Info("Instance created");
	}
	return mInstance;
}

void Renderer::DestroyInstance() {
	RenderLog::Warn("Instance destruction requested");
	if (mInstance != nullptr) {
		if (mInstance->GetState() != RendererState::CLEANUP) {
			mInstance->Cleanup();
		}

		RenderLog::Info("Exiting...");
		mInstance->mState = RendererState::EXIT;
	}
}

std::vector<unsigned int> VAOs = {};
std::vector<unsigned int> VBOs = {};

Math::Matrix4 projectionMatrix;
std::chrono::steady_clock::time_point timeLast;

void Renderer::InitWindow() {
	RenderLog::Info("Instantiating window...");
	mWindow = new Platform::Window();
	mWindow->Init();
}

void Renderer::Init() {
	mState = RendererState::INIT;
	RenderLog::Info("Initializing...");

	mViewportRect = Math::Rect(Settings::CurrentSettings->Window.Width, Settings::CurrentSettings->Window.Height);
	mViewportRectLast = mViewportRect;

	auto window = mWindow->GetNativeWindow();
	GLFWwindow* window2 = (GLFWwindow*)window;
	glfwMakeContextCurrent(window2);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	auto result = glfwGetCurrentContext();

	RenderLog::Info("Instantiating camera...");
	mCamera = new BaseCamera();
	mWindow->SetCurrentCamera(mCamera);
	mCamera->mFrustum.w = mViewportRect.w;
	mCamera->mFrustum.h = mViewportRect.h;

	Assets::Texture::EngineTexturesPath = Constants::GetResourcePath() + "textures/";

	RenderLog::Info("Loading shaders...");
	Assets::Shader::LoadAllShaders();

	mGeomPassShader = Assets::Shader::GetShaderByName("gbufferShader");
	mLightingPassShader = Assets::Shader::GetShaderByName("lightingShader");

	mLightingPassShader->Activate();
	mLightingPassShader->SetUniformInt("gPosition", 0);
	mLightingPassShader->SetUniformInt("gNormal", 1);
	mLightingPassShader->SetUniformInt("gAlbedoSpec", 2);

	RenderLog::Info("Creating G-Buffer...");
	mGBuffer = new GBuffer();
	if (!mGBuffer->Init(mViewportRect.w, mViewportRect.h)) throw;

	RenderLog::Info("Creating uniform buffer object...");
	projectionMatrix = Math::Matrix4::Perspective(mCamera->mFrustum);
	sUBO initData = {
		Utilities::NativeToGLMMat4(mCamera->GetViewMatrix()),
		Utilities::NativeToGLMMat4(projectionMatrix)
	};
	mUBO = new UniformBufferObject(initData);

	RenderLog::Info("Loading test scene...");
	mLoadedScene = new BaseScene();

	RenderLog::Info("Initialisation complete");

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

void Renderer::Run() {
	mStartRenderTime = std::chrono::steady_clock::now();
	timeLast = mStartRenderTime;
	mState = RendererState::RUNNING;
	GLFWwindow* windowInstance = (GLFWwindow*)mWindow->GetNativeWindow();

	if (!ImGui_ImplGlfw_InitForOpenGL(windowInstance, true)) throw std::runtime_error("Failed to init ImGui for GLFW");
	if (!ImGui_ImplOpenGL3_Init("#version 330")) throw std::runtime_error("Failed to init ImGui for OpenGL");

	mWindow->mImGuiImpl->mSelectedObject = mLoadedScene->mNyen;

	while (!glfwWindowShouldClose(windowInstance)) {
		auto timeNow = std::chrono::steady_clock::now();
		mDeltaRenderTime = std::chrono::duration<double>(timeNow - timeLast).count();
		mElapsedRenderTime = std::chrono::duration<double>(timeNow - mStartRenderTime).count();
		timeLast = timeNow;

		mLoadedScene->Tick((float)mDeltaRenderTime);

		Components::Mesh::FrameMeshCount = 0;
		Components::Mesh::FrameVertexCount = 0;
		mWindow->mImGuiImpl->mDebugValues.elapsedTime = (float)mElapsedRenderTime;
		mWindow->mImGuiImpl->mDebugValues.deltaTime = (float)mDeltaRenderTime;
		mWindow->mImGuiImpl->mDebugValues.fps = (float)(1.0 / mDeltaRenderTime);
		mWindow->mImGuiImpl->mDebugValues.cameraGridIndex = mCamera->mTransform.mSpatialPosition.GridIndex;
		mWindow->mImGuiImpl->mDebugValues.cameraCellPos = mCamera->mTransform.mSpatialPosition.CellPosition;
		mWindow->mImGuiImpl->mDebugValues.cameraWorldPos = mCamera->mTransform.GetWorldPosition();
		mWindow->OnUpdate();

		if (mShouldRender) {
			UpdateUniformBuffers();
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// [Geometry Pass] //
			//-----------------//
			DSPassGeometry();
			DSPassLighting();
			DSPassFinal();


			// GUI
			mWindow->mImGuiImpl->mDebugValues.meshCount = Components::Mesh::FrameMeshCount;
			mWindow->mImGuiImpl->mDebugValues.vertexCount = Components::Mesh::FrameVertexCount;
			mWindow->mImGuiImpl->Draw();
			mEditorInterfaceDrawCallback();

			// [Finalise Tick] //
			//-----------------//
			glfwSwapBuffers(windowInstance);
		}
		glfwPollEvents();
	}

	mInstance->Cleanup();
}

void Renderer::UpdateUniformBuffers() const {
	sUBO newData{};
	newData.viewMatrix = Utilities::NativeToGLMMat4(mCamera->GetViewMatrix());

	if (mViewportRect != mViewportRectLast) {
		if (!mGBuffer->Regenerate(mViewportRect.w, mViewportRect.h)) throw;
		glViewport(mViewportRect.x, mViewportRect.y, mViewportRect.w, mViewportRect.h);
		mCamera->mFrustum.w = mViewportRect.w;
		mCamera->mFrustum.h = mViewportRect.h;
		projectionMatrix = Math::Matrix4::Perspective(mCamera->mFrustum);
	}
	newData.perspectiveMatrix = Utilities::NativeToGLMMat4(projectionMatrix);
	mUBO->UploadNewData(newData);
}

void Renderer::Cleanup() {
	mState = RendererState::CLEANUP;

	RenderLog::Info("Cleaning up...");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}


// Deferred Shading

void Renderer::DSPassGeometry() const {
	mGBuffer->BindFull();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mGeomPassShader->Activate();

	if (Settings::CurrentSettings->Graphics.WireframeEnabled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Draw models
	mLoadedScene->Render();

	// Draw billboards
	Billboard::DrawAll();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (Settings::CurrentSettings->Graphics.WireframeEnabled) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void Renderer::DSPassLighting() const {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mGBuffer->BindTextures();

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
	mGBuffer->BindForRead();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	int viewX0 = mViewportRect.x;
	int viewY0 = mViewportRect.y;
	int viewX1 = viewX0 + mViewportRect.w;
	int viewY1 = viewY0 + mViewportRect.h;
	glBlitFramebuffer(0, 0, mViewportRect.w, mViewportRect.h, viewX0, viewY0, viewX1, viewY1, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
