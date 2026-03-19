#pragma once

#include "../ShaderManager.h"
#include "BaseModel.h"

class BaseLight
{
public:
	float mLightIntensity = 1.0f;
	glm::vec3 mLightColor = glm::vec3(1.0f);

	BaseShader* mLightShader;
	Transform* mTransform;

	BaseLight();

	void UpdateShaderUniforms(unsigned int index);
private:
};

