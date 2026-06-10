#include "Material.h"

void Refraction::Assets::Material::Activate() {
	mDiffuse->Activate(0);
	mSpecular->Activate(1);
	//mNormal->Activate(2);

	mShader->SetUniformInt(REFRACT_TEXTURE_TYPE_DIFFUSE + std::string("1"), 0);
	mShader->SetUniformInt(REFRACT_TEXTURE_TYPE_SPECULAR + std::string("1"), 1);

	mShader->Activate();
}
