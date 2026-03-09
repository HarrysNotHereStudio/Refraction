#include "OpenGLRenderer.h"

#pragma warning(push, 0)
#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"
#pragma warning(pop)

#define VIEW_WIDTH this->m_currentSettings.window.windowWidth
#define VIEW_HEIGHT this->m_currentSettings.window.windowHeight

OpenGLRenderer* OpenGLRenderer::m_pInstance = nullptr;

OpenGLRenderer::OpenGLRenderer() {}

OpenGLRenderer* OpenGLRenderer::getInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new OpenGLRenderer();
		Log::Info("Instance created");
	}
	return m_pInstance;
}

void OpenGLRenderer::destroyInstance()
{
	Log::Info("Instance destruction requested");
	if (m_pInstance != nullptr)
	{
		if (m_pInstance->getState() != OpenGLRendererState::CLEANUP) {
			m_pInstance->cleanup();
		}

		Log::Info("Exiting...");
		m_pInstance->m_state = OpenGLRendererState::EXIT;
	}
}

std::string shadersPath = "Rendering/Resources/shaders";
std::vector<BaseShader*> LoadedShaders = {};

void OpenGLRenderer::loadShaders() {
	using std::string, std::vector, std::filesystem::directory_entry;

	vector<directory_entry> shaderSources = Utilities::getFoldersInFolder(shadersPath);

	for (const auto& shaderSource : shaderSources) {
		string shaderSourcePath = shaderSource.path().string();
		Log::Info("Loading shader source: " + shaderSourcePath);
		BaseShader* newShader = new BaseShader(shaderSourcePath);
		LoadedShaders.push_back(newShader);
	}
}

BaseShader* getShaderByName(const std::string name) {
	for (const auto& shader : LoadedShaders) {
		if (shader->m_name == name) {
			return shader;
		}
	}
}

std::vector<unsigned int> VAOs = {};
std::vector<unsigned int> VBOs = {};
std::vector<BaseModel*> LoadedModels = {};

glm::mat4 projectionMatrix;

int OpenGLRenderer::init(Settings* initSettings) {
	m_state = OpenGLRendererState::INIT;
	Log::Info("Initializing...");
	m_currentSettings = *initSettings;

	Log::Info("Instantiating window...");
	glfwInit();
	m_pWindow = new Window();
	m_pWindow->init(initSettings->window);

	glEnable(GL_DEPTH_TEST);

	Log::Info("Instantiating camera...");
	m_pCamera = new BaseCamera();
	m_pCamera->setCameraSpeed(m_currentSettings.controls.cameraSpeed);
	m_pCamera->setCameraSensitivity(m_currentSettings.controls.cameraSensitivity);

	Log::Info("Loading shaders...");
	// Prepare shaders
	loadShaders();

	m_pLightingPassShader = getShaderByName("lightingPassShader");

	Log::Info("Creating G-Buffer...");
	m_pGBuffer = new GBuffer();
	m_pGBuffer->Init(VIEW_WIDTH, VIEW_HEIGHT);

	m_pLightingPassShader->activate();
	m_pLightingPassShader->setUniformInt("gPosition", 0);
	m_pLightingPassShader->setUniformInt("gNormal", 1);
	m_pLightingPassShader->setUniformInt("gDiffuseSpecular", 2);


	Log::Info("Creating uniform buffer object...");
	float aspectRatio = VIEW_WIDTH / static_cast<float>(VIEW_HEIGHT);
	projectionMatrix = glm::perspective(glm::radians(70.0f), aspectRatio, m_currentSettings.graphics.clipPlaneNear, m_currentSettings.graphics.clipPlaneFar);
	sUBO initData = {
		m_pCamera->m_pTransform->GetTransform(),
		m_pCamera->m_pTransform->position,
		projectionMatrix
	};
	m_pUBO = new UniformBufferObject(initData);

	Log::Info("Creating test model...");
	BaseModel* testModel = new BaseModel("./Rendering/Resources/models/survivalBackpack/backpack.obj");
	testModel->m_pShader = getShaderByName("baseShader");
	LoadedModels.push_back(testModel); // asshole

	BaseModel* groundPlane = new BaseModel("./Rendering/Resources/models/Primitives/primitivePlane.obj");
	groundPlane->m_pShader = getShaderByName("baseShader");
	groundPlane->m_pTransform->Translate(glm::vec3(0.0f, -10.0f, 0.0f));
	groundPlane->m_pTransform->Scale(glm::vec3(10.0f));
	LoadedModels.push_back(groundPlane);

	Log::Info("Creating test light...");
	PointLight* newLight = new PointLight();
	newLight->m_pLightModel->m_pShader = getShaderByName("lightShader");
	getShaderByName("baseShader")->activate();
	newLight->setLightColor(glm::vec3(1.0f));
	m_pScenePointLights.push_back(newLight);
	
	Log::Info("Initialisation complete");

	m_startRenderTime = glfwGetTime();
	mainLoop();

	return 0;
}

void updateLightShaderUniforms(BaseShader* pLightingPassShader, vector<BaseLight*> lights) {
	pLightingPassShader->setUniformInt("LIGHTCOUNT", lights.size());

	for (unsigned int i = 0; i < static_cast<int>(lights.size()); i++) {
		const auto& light = lights[i];
		pLightingPassShader->setUniformVec3("lights[" + std::to_string(i) + "].Position", light->m_pLightModel->m_pTransform->position);
		pLightingPassShader->setUniformVec3("lights[" + std::to_string(i) + "].Color", light->getLightColor());
	}
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
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
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void OpenGLRenderer::mainLoop() {
	m_state = OpenGLRendererState::RUNNING;
	double fpsPrintDelta = 0;
	GLFWwindow* windowInstance = m_pWindow->getWindow();

	while (!glfwWindowShouldClose(windowInstance))
	{
		m_deltaRenderTime = glfwGetTime() - m_elapsedRenderTime;
		m_elapsedRenderTime = glfwGetTime() - m_startRenderTime;
		if (fpsPrintDelta > 1000) {
			fpsPrintDelta = 0;
			Log::Info(std::to_string(m_elapsedRenderTime) + " elapsed | " + std::to_string(m_deltaRenderTime) + " delta | " + Utilities::calculateFPS(m_deltaRenderTime, 2) + "FPS");
		};
		fpsPrintDelta += m_deltaRenderTime;

		if (m_shouldRender) {
			LoadedShaders[0]->setUniformVec4("ourColor", 0.0f, static_cast<float>(sin(m_elapsedRenderTime)), 0.0f, 1.0f);
			updateUniformBuffers();

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			// Draw models
			for (int i = 0; i < LoadedModels.size(); i++) {
				LoadedModels[i]->DrawModel();
			}
			renderQuad();
			/*

			// [Geometry Pass] //
			//-----------------//
			DSPassGeometry();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			// [Stencil & Lighting Pass] //
			//---------------------------//
			for (unsigned int i = 0; i < m_pScenePointLights.size(); i++) {
				PointLight* pPointLight = m_pScenePointLights[i];
				DSPassStencil(pPointLight);
				DSPassPointLight(pPointLight);
			}

			glDisable(GL_STENCIL_TEST);

			DSPassDirectionalLight();

			DSPassFinal();
			
			*/
			// [Finalise Tick] //
			//-----------------//
			glfwSwapBuffers(windowInstance);
		}
		glfwPollEvents();
	}

	m_pInstance->cleanup();
}

void OpenGLRenderer::updateUniformBuffers() {
	glm::mat4 viewMatrix = m_pCamera->m_pTransform->GetTransform();
	glm::vec3 viewPos = m_pCamera->m_pTransform->position;
	sUBO newData = {
		viewMatrix,
		viewPos,
		projectionMatrix
	};
	m_pUBO->UploadNewData(newData);
}

void OpenGLRenderer::cleanup() {
	m_state = OpenGLRendererState::CLEANUP;
	Log::Info("Cleaning up...");
	glfwTerminate();
}


// Deferred Shading

void OpenGLRenderer::DSPassGeometry() {
	m_pGBuffer->BindGeomPass();
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// Draw models
	for (int i = 0; i < LoadedModels.size(); i++) {
		LoadedModels[i]->DrawModel();
	}
	m_pScenePointLights[0]->m_pLightModel->DrawModel();

	glDepthMask(GL_FALSE);
}

void OpenGLRenderer::DSPassStencil(PointLight* pPointLight) {
	pPointLight->m_pLightModel->m_pShader = getShaderByName("stencilPassShader");
	m_pGBuffer->BindStencilPass();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	pPointLight->m_pLightModel->DrawModel();
}

void OpenGLRenderer::DSPassPointLight(PointLight* pPointLight) {
	pPointLight->m_pLightModel->m_pShader = getShaderByName("lightingPassShader");
	m_pGBuffer->BindLightPass();

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	pPointLight->m_pLightModel->DrawModel();
	glCullFace(GL_BACK);

	glDisable(GL_BLEND);
}

void OpenGLRenderer::DSPassDirectionalLight() {
	m_pGBuffer->BindLightPass();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glDisable(GL_BLEND);
}

void OpenGLRenderer::DSPassFinal() {
	m_pGBuffer->BindFinalPass();
	glBlitFramebuffer(0, 0, VIEW_WIDTH, VIEW_HEIGHT, 0, 0, VIEW_WIDTH, VIEW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}