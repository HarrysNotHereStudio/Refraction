#pragma once

#include <string>

#include <Platform/ATexture.h>

#include "Asset.h"

constexpr auto RFCT_TEXTURE_TYPE_DIFFUSE = "tDiffuse";
constexpr auto RFCT_TEXTURE_TYPE_SPECULAR = "tSpecular";

namespace Refraction::Assets {
	struct ImageMetadata : public AssetMetadata {
		int Width = 0;
		int Height = 0;
		int Channels = 3;
		std::string Type = RFCT_TEXTURE_TYPE_DIFFUSE;

		ImageMetadata() = default;
		~ImageMetadata() = default;

		std::string Serialise() override;
		void Deserialise(std::string data) override;
	};

	class Image : public Asset {
	public:
		Common::Shared<Engine::Platform::ATexture> mTexture = nullptr;

		Image() = default;
		virtual ~Image();
	protected:
		void InternalLoadAsset(Common::Shared<AssetMetadata> metadata) override;
	private:
		unsigned int mID = 0;
	};
}
