#pragma once

#include <Core/Common.h>
#include <Math/Transform.h>
#include <Classes/Assets/Shader.h>

class BaseLight
{
public:
	float mLightIntensity = 1.0f;
	Refraction::Math::Vector3 mLightColor = Refraction::Math::Vector3(1.0f);

	Refraction::Common::Ref<Refraction::Assets::Shader> mLightShader = {};
	Refraction::Math::Transform* mTransform = nullptr;

	BaseLight();

	void UpdateShaderUniforms(unsigned int index);
private:
};
