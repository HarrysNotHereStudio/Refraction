
#include "BaseLight.h"

namespace RMath = Refraction::Math;
using RMath::Transform;

BaseLight::BaseLight() {
	mLightShader = Refraction::Assets::Shader::GetShaderByName("lightingShader");
	mTransform = new Transform();
	mBillboard = new Billboard();
	mBillboard->mTransform = mTransform;
	mBillboard->SetTexture(Refraction::Assets::Texture::EngineTexturesPath + "Icons_Light.png");
}

void BaseLight::UpdateShaderUniforms(unsigned int index) {
	const float linear = 0.8f;
	const float quadratic = 0.4f;
	const float radius = 200.0f;
	mLightShader->SetUniformVec3("lights[" + std::to_string(index) + "].Position", mTransform->GetWorldPosition());
	mLightShader->SetUniformVec3("lights[" + std::to_string(index) + "].Color", mLightColor);
	mLightShader->SetUniformFloat("lights[" + std::to_string(index) + "].Linear", linear);
	mLightShader->SetUniformFloat("lights[" + std::to_string(index) + "].Quadratic", quadratic);
	mLightShader->SetUniformFloat("lights[" + std::to_string(index) + "].Radius", radius);
}