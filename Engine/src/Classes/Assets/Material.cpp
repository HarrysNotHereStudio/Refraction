#include <Interface/AssetManager.h>

#include "Material.h"

namespace Refraction::Assets {
	Material::Material() {
		Engine::AssetManager::Try([&](Common::Shared<Engine::AssetManager> assetManager) {
			mDiffuse = assetManager->GetAsset<Assets::Image>("Basic.png");
			mSpecular = assetManager->GetAsset<Assets::Image>("Basic.png");
			mShader = assetManager->GetAsset<Assets::Shader>("gbufferShader");
		});
	}

	void Material::Activate() {
		if (auto img = mDiffuse.lock()) {
			if (auto tex = img->mTexture.lock()) {
				tex->Activate(0);
			}
		}
		if (auto img = mSpecular.lock()) {
			if (auto tex = img->mTexture.lock()) {
				tex->Activate(1);
			}
		}
		//mNormal->Activate(2);

		if (auto shader = mShader.lock()) {
			shader->SetUniformInt(RFCT_TEXTURE_TYPE_DIFFUSE, 0);
			shader->SetUniformInt(RFCT_TEXTURE_TYPE_SPECULAR, 1);

			shader->Activate();
		}
	}

}
