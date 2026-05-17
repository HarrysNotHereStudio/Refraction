#pragma once

#include "Asset.h"
#include "Texture.h"
#include "Shader.h"

namespace EngineAssets {
	class Material : public Asset {
	public:
		EngineAssets::Shader* mShader;
		EngineAssets::Texture* mDiffuse;
		EngineAssets::Texture* mSpecular;
		EngineAssets::Texture* mNormal;

		void Activate();
	};

}