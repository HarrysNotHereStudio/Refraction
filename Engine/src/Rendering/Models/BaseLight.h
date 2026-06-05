#pragma once

#include "../ShaderManager.h"
#include "Billboard.h"

class BaseLight
{
public:
	float mLightIntensity = 1.0f;
	Refraction::Math::Vector3 mLightColor = Refraction::Math::Vector3(1.0f);

	EngineAssets::Shader* mLightShader = nullptr;
	Refraction::Math::Transform* mTransform = nullptr;

	BaseLight();

	void UpdateShaderUniforms(unsigned int index);
private:
	Billboard* mBillboard = nullptr;
};
