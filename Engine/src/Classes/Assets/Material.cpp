#include "Material.h"

void Refraction::Assets::Material::Activate() {
	if (mDiffuse && mDiffuse->mTexture) mDiffuse->mTexture->Activate(0);
	if (mSpecular && mSpecular->mTexture) mSpecular->mTexture->Activate(1);
	//mNormal->Activate(2);

	mShader->SetUniformInt(RFCT_TEXTURE_TYPE_DIFFUSE, 0);
	mShader->SetUniformInt(RFCT_TEXTURE_TYPE_SPECULAR, 1);

	mShader->Activate();
}
