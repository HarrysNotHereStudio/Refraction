#pragma once

#include <string>

#include <EngineClasses/Assets/Shader.h>

class ShaderManager {
public:
	ShaderManager() = default;

	static void LoadAllShaders();

	static EngineAssets::Shader* GetShaderByName(const std::string name);
};

