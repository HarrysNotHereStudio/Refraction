#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#pragma warning(push)
#pragma warning(disable : 26819)
#pragma warning(disable : 6262)
#include "STB/stb_image.h"
#pragma warning(pop)

#include "Texture.h"

namespace Refraction::Assets {
	std::unordered_map<std::string, Texture*> LoadedTextures;

	std::string TextureMetadata::Serialise() {
		auto result = AssetMetadata::Serialise();
		return result;
	}

	void TextureMetadata::Deserialise(std::string data) {
		AssetMetadata::Deserialise(data);
	}


	Texture* Texture::GetTexture(std::filesystem::path texturePath, std::string textureType) {
		if (LoadedTextures.contains(texturePath.string())) return LoadedTextures.at(texturePath.string());
		
		Texture* newTexture = new Texture();
		newTexture->LoadAsset(texturePath);
		newTexture->mMetadata.Type = textureType;
		LoadedTextures[texturePath.string()] = newTexture;

		return newTexture;
	}

	Texture::Texture(unsigned int id) {
		if (!glIsTexture(id)) {
			Log::Render.Error("Attempted to create a Texture with an invalid ID");
			throw;
		}
		mID = id;

		// Gather metadata
		glBindTexture(GL_TEXTURE_2D, mID);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &mMetadata.Width);
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &mMetadata.Height);
		//glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPONENTS, &mMetadata.Channels);
		mMetadata.Channels = 3; // hardcode this shi for now
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Activate(unsigned int unitOffset) {
		glActiveTexture(GL_TEXTURE0 + unitOffset);
		glBindTexture(GL_TEXTURE_2D, mID);
	}


	void Texture::LoadAsset(const std::filesystem::path& source) {
		Asset::LoadAsset(source);

		glGenTextures(1, &mID);
		glBindTexture(GL_TEXTURE_2D, mID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(mMetadata.AssetPath.string().c_str(), &mMetadata.Width, &mMetadata.Height, &mMetadata.Channels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mMetadata.Width, mMetadata.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			Log::Render.Info("Created texture from path " + mMetadata.AssetPath.string());
		} else {
			Log::Render.Error("Failed to create texture from path " + mMetadata.AssetPath.string());
		}
		stbi_image_free(data);
	}
}
