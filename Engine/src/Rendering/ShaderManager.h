#pragma once

#include <string>

#include "Graphics/BaseShader.h"

class ShaderManager {
public:
	ShaderManager() = default;

	static void LoadAllShaders(std::string resourcesPath);

	static BaseShader* GetShaderByName(const std::string name);
};

