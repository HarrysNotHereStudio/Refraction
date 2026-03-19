#pragma once

#include <string>

#include "Graphics/BaseShader.h"

static class ShaderManager {
public:
	ShaderManager() = default;

	static void LoadAllShaders();

	static BaseShader* GetShaderByName(const std::string name);
};

