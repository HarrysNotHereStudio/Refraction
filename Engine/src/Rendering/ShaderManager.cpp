#include "Constants.h"

#include "ShaderManager.h"


std::string shadersSubPath = "/shaders";
std::vector<BaseShader*> LoadedShaders = {};

void ShaderManager::LoadAllShaders(std::string resourcesPath) {
	using std::string, std::vector, std::filesystem::directory_entry;

	Log::Info("Loading all shaders");
	vector<directory_entry> shaderSources = Utilities::getFoldersInFolder(resourcesPath + shadersSubPath);

	for (const auto& shaderSource : shaderSources) {
		string shaderSourcePath = shaderSource.path().string();
		Log::Info("Loading shader source: " + shaderSourcePath);
		BaseShader* newShader = new BaseShader(shaderSourcePath);
		LoadedShaders.push_back(newShader);
	}
}

BaseShader* ShaderManager::GetShaderByName(const std::string name) {
	for (const auto& shader : LoadedShaders) {
		if (shader->mName == name) {
			return shader;
		}
	}
	throw std::runtime_error("Could not get shader of name " + name);
}