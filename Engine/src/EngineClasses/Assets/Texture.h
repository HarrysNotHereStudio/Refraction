#pragma once

#include <string>

#include "Asset.h"

#define REFRACT_TEXTURE_TYPE_DIFFUSE "texDiffuse"
#define REFRACT_TEXTURE_TYPE_SPECULAR "texSpecular"

namespace EngineAssets {
	class Texture : public Asset {
	public:
		static std::string EngineTexturesPath;

		// Loads a texture from disk (or returns a pointer to the existing texture)
		static Texture* GetTexture(std::string texturePath, std::string textureType);
		void Activate(unsigned int unitOffset);

		std::string GetTextureType() { return mTextureType; };
		std::string GetSourcePath() { return mSourcePath; };
	private:
		Texture(std::string texturePath, std::string textureType);

		unsigned int mID;
		std::string mSourcePath;
		std::string mTextureType;
	};
}
