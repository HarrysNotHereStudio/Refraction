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
		Common::Ref<Engine::Platform::ATexture> mTexture = nullptr;

		// Loads a texture from disk (or returns a pointer to the existing texture)
		static Common::Ref<Image> FromPath(std::filesystem::path texturePath);

		Image() = default;
		virtual ~Image();

		ImageMetadata& GetMetadata() override { return mMetadata; }
	protected:
		void LoadAsset(const std::filesystem::path& source) override;
	private:
		ImageMetadata mMetadata;
		unsigned int mID = 0;
	};
}
