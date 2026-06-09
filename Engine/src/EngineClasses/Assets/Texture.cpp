#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma warning(push)
#pragma warning(disable : 26819)
#pragma warning(disable : 6262)
#include "STB/stb_image.h"
#pragma warning(pop)

#include <Core/Log.h>

#include "Texture.h"

std::unordered_map<std::string, EngineAssets::Texture*> LoadedTextures;
std::string EngineAssets::Texture::EngineTexturesPath = "";


EngineAssets::Texture* EngineAssets::Texture::GetTexture(std::string texturePath, std::string textureType) {
	if (LoadedTextures.contains(texturePath)) return LoadedTextures.at(texturePath);

	Texture* newTexture = new Texture(texturePath, textureType);
	LoadedTextures[texturePath] = newTexture;

	return newTexture;
}

void EngineAssets::Texture::Activate(unsigned int unitOffset) {
	glActiveTexture(GL_TEXTURE0 + unitOffset);
	glBindTexture(GL_TEXTURE_2D, mID);
}


EngineAssets::Texture::Texture(std::string texturePath, std::string textureType) {
	mSourcePath = texturePath;
	mTextureType = textureType;

	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(mSourcePath.c_str(), &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		Refraction::RuntimeLog::Info("Created texture from path " + mSourcePath);
	} else {
		Refraction::RuntimeLog::Error("Failed to create texture from path " + mSourcePath);
	}
	stbi_image_free(data);
}
