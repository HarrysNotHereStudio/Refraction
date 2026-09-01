#pragma once

#include "Asset.h"
#include "Image.h"
#include "Shader.h"

namespace Refraction::Assets {
	class Material : public Asset {
	public:
		Common::Shared<Shader> mShader;
		Common::Shared<Image> mDiffuse;
		Common::Shared<Image> mSpecular;
		Common::Shared<Image> mNormal;

		Material();

		void Activate();
	};

}