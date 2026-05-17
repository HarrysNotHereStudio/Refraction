#include <vector>

#include <EngineConstants.h>
#include <EngineLog.h>
#include <EngineUtilities.h>

#include "ShaderManager.h"


std::string shadersSubPath = "shaders/";
std::vector<EngineAssets::Shader*> LoadedShaders = {};

void ShaderManager::LoadAllShaders() {
	using std::string, std::vector, std::filesystem::directory_entry;

	RenderLog::Info("Loading all shaders");
	vector<directory_entry> shaderSources = Utilities::GetFoldersInFolder(EngineConstants::GetResourcePath() + shadersSubPath);

	for (const auto& shaderSource : shaderSources) {
		string shaderSourcePath = shaderSource.path().string();
		RenderLog::Info("Loading shader source: " + shaderSourcePath);
		EngineAssets::Shader* newShader = new EngineAssets::Shader(shaderSourcePath);
		LoadedShaders.push_back(newShader);
	}
}

EngineAssets::Shader* ShaderManager::GetShaderByName(const std::string name) {
	for (const auto& shader : LoadedShaders) {
		if (shader->GetName() == name) {
			return shader;
		}
	}
	throw std::runtime_error("Could not get shader of name " + name);
}