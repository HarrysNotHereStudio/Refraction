#pragma once

#include <Classes/Assets/Texture.h>
#include "AComponent.h"

namespace Refraction::Components {
	class Billboard : public AComponent {
	public:
		Math::Transform mTransform;
		Common::Ref<Assets::Texture> mTexture;
		bool mRenderOnTop = false;

		Billboard();

		void Render() override;

		std::string Serialise() override;
		void Deserialise(std::string serialised) override;
	private:
	};

}