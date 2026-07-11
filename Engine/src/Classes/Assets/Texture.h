#pragma once

#include <string>

#include "Asset.h"

#define REFRACT_TEXTURE_TYPE_DIFFUSE "texDiffuse"
#define REFRACT_TEXTURE_TYPE_SPECULAR "texSpecular"

namespace Refraction::Assets {
	struct TextureMetadata : public AssetMetadata {
		int Width = 0;
		int Height = 0;
		int Channels = 0;
		std::string Type = "texDiffuse";

		std::string Serialise() override;
		void Deserialise(std::string data) override;
	};

	class Texture : public Asset {
	public:
		// Loads a texture from disk (or returns a pointer to the existing texture)
		static Texture* GetTexture(std::filesystem::path texturePath, std::string textureType);
		// Creates a texture using an existing ID (NOTE: texture must already be loaded)
		Texture(unsigned int id);

		void Activate(unsigned int unitOffset);

		unsigned int GetID() const { return mID; }
		TextureMetadata& GetMetadata() override { return mMetadata; }
	protected:

		void LoadAsset(const std::filesystem::path& source) override;
	private:
		TextureMetadata mMetadata;
		unsigned int mID = 0;

		Texture() {}
	};
}
