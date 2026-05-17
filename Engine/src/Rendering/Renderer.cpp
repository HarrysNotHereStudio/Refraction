
#include "Models/Billboard.h"

#include "Renderer.h"

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"
#pragma warning(pop)

#define VIEW_WIDTH Settings::CurrentSettings->Window.Width
#define VIEW_HEIGHT Settings::CurrentSettings->Window.Height

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

ImGuiStyle GetDefaultStyle() {
	ImGuiStyle style = ImGuiStyle();
	style.WindowPadding = ImVec2(6, 6);
	style.FramePadding = ImVec2(2, 2);
	style.ItemSpacing = ImVec2(8, 2);
	style.ItemInnerSpacing = ImVec2(4, 2);
	style.WindowBorderSize = 1;
	style.ChildBorderSize = 1;
	style.PopupBorderSize = 1;
	style.FrameBorderSize = 0;
	style.WindowRounding = 1;
	style.ChildRounding = 0;
	style.FrameRounding = 0;
	style.PopupRounding = 0;
	style.GrabRounding = 0;
	style.ScrollbarSize = 8;
	style.ScrollbarRounding = 0;
	style.ScrollbarPadding = 2;
	style.TabBorderSize = 0;
	style.TabBarBorderSize = 1;
	style.TabRounding = 0;

	return style;
}

std::vector<unsigned int> VAOs = {};
std::vector<unsigned int> VBOs = {};

glm::mat4 projectionMatrix;
std::chrono::steady_clock::time_point timeLast;

int Renderer::Init() {
	mState = RendererState::INIT;
	RenderLog::Info("Initializing...");
	RenderLog::Info("Resource path: " + EngineConstants::GetResourcePath());

	RenderLog::Info("Instantiating window...");
	glfwInit();
	mWindow = new Window();
	mWindow->Init();


	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	RenderLog::Info("Instantiating camera...");
	mCamera = new BaseCamera();
	//mCamera->SetCameraSpeed(mCurrentSettings.controls.cameraSpeed*6767676767676767);
	//mCamera->SetCameraSensitivity(mCurrentSettings.controls.cameraSensitivity*414141414141);

	EngineAssets::Texture::EngineTexturesPath = EngineConstants::GetResourcePath() + "textures/";

	RenderLog::Info("Loading shaders...");
	ShaderManager::LoadAllShaders();

	mGeomPassShader = ShaderManager::GetShaderByName("gbufferShader");
	mLightingPassShader = ShaderManager::GetShaderByName("lightingShader");

	mLightingPassShader->Activate();
	mLightingPassShader->SetUniformInt("gPosition", 0);
	mLightingPassShader->SetUniformInt("gNormal", 1);
	mLightingPassShader->SetUniformInt("gAlbedoSpec", 2);

	RenderLog::Info("Creating G-Buffer...");
	mGBuffer = new GBuffer();
	mGBuffer->Init(VIEW_WIDTH, VIEW_HEIGHT);

	RenderLog::Info("Creating uniform buffer object...");
	float aspectRatio = VIEW_WIDTH / static_cast<float>(VIEW_HEIGHT);
	projectionMatrix = glm::perspective(glm::radians(mCamera->mFOVy), aspectRatio, Settings::CurrentSettings->Graphics.ClipPlaneNear, Settings::CurrentSettings->Graphics.ClipPlaneFar);
	sUBO initData = {
		mCamera->GetViewMatrix(),
		projectionMatrix
	};
	mUBO = new UniformBufferObject(initData);

	RenderLog::Info("Loading test scene...");
	mLoadedScene = new BaseScene();

	RenderLog::Info("Initialising ImGui...");
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui::GetStyle() = GetDefaultStyle();

	RenderLog::Info("Initialisation complete");

	mStartRenderTime = std::chrono::steady_clock::now();
	timeLast = mStartRenderTime;
	MainLoop();

	return 0;
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

void Renderer::MainLoop() {
	mState = RendererState::RUNNING;
	GLFWwindow* windowInstance = mWindow->GetWindow();

	if (!ImGui_ImplGlfw_InitForOpenGL(windowInstance, true)) throw std::runtime_error("Failed to init ImGui for GLFW");
	if (!ImGui_ImplOpenGL3_Init("#version 330")) throw std::runtime_error("Failed to init ImGui for OpenGL");

	while (!glfwWindowShouldClose(windowInstance)) {
		auto timeNow = std::chrono::steady_clock::now();
		mDeltaRenderTime = std::chrono::duration<double>(timeNow - timeLast).count();
		mElapsedRenderTime = std::chrono::duration<double>(timeNow - mStartRenderTime).count();
		timeLast = timeNow;

		mLoadedScene->Tick((float)mDeltaRenderTime);

		mWindow->mDebugValues.elapsedTime = (float)mElapsedRenderTime;
		mWindow->mDebugValues.deltaTime = (float)mDeltaRenderTime;
		mWindow->mDebugValues.fps = (float)(1.0 / mDeltaRenderTime);
		mWindow->UpdateLoop();

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
			mWindow->DrawGUI();

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
	newData.viewMatrix = mCamera->GetViewMatrix();
	newData.perspectiveMatrix = projectionMatrix;
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
	mGBuffer->BindAny();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	mGeomPassShader->Activate();

	if (mWindow->mWireframeToggle) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	// Draw models
	for (const auto& mModel : mLoadedScene->mModels) {
		mModel->DrawModel();
	}

	// Draw billboards
	Billboard::DrawAll();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (mWindow->mWireframeToggle) {
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
	mLightingPassShader->SetUniformVec3("viewPos", mCamera->mTransform.position);
	// finally render quad
	renderQuad();
}

void Renderer::DSPassFinal() const {
	mGBuffer->BindForRead();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBlitFramebuffer(0, 0, VIEW_WIDTH, VIEW_HEIGHT, 0, 0, VIEW_WIDTH, VIEW_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
