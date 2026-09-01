#include <Interface/AssetManager.h>

#include "Material.h"

namespace Refraction::Assets {
	Material::Material() {
		auto assetManager = Engine::AssetManager::GetInstance();
		if (assetManager) {
			mDiffuse = assetManager->GetAsset<Assets::Image>("Basic.png");
			mSpecular = assetManager->GetAsset<Assets::Image>("Basic.png");
			mShader = assetManager->GetAsset<Assets::Shader>("gbufferShader");
		}
	}

	void Material::Activate() {
		if (mDiffuse && mDiffuse->mTexture) mDiffuse->mTexture->Activate(0);
		if (mSpecular && mSpecular->mTexture) mSpecular->mTexture->Activate(1);
		//mNormal->Activate(2);

		mShader->SetUniformInt(RFCT_TEXTURE_TYPE_DIFFUSE, 0);
		mShader->SetUniformInt(RFCT_TEXTURE_TYPE_SPECULAR, 1);

		mShader->Activate();
	}

}
