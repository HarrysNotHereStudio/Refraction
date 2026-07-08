#pragma once

#include "Asset.h"
#include "Texture.h"
#include "Shader.h"

namespace Refraction::Assets {
	class Material : public Asset {
	public:
		Shader* mShader = nullptr;
		Texture* mDiffuse = nullptr;
		Texture* mSpecular = nullptr;
		Texture* mNormal = nullptr;

		void Activate();
	};

}