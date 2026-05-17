#pragma once

#include "../ShaderManager.h"
#include "Billboard.h"

class BaseLight
{
public:
	float mLightIntensity = 1.0f;
	glm::vec3 mLightColor = glm::vec3(1.0f);

	EngineAssets::Shader* mLightShader = nullptr;
	Transform* mTransform = nullptr;

	BaseLight();

	void UpdateShaderUniforms(unsigned int index);
private:
	Billboard* mBillboard = nullptr;
};
