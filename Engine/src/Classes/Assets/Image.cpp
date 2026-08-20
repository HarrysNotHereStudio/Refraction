#include <unordered_map>

#include "Image.h"

namespace Refraction::Assets {
	std::unordered_map<std::string, Image*> LoadedImages;

	std::string ImageMetadata::Serialise() {
		auto result = AssetMetadata::Serialise();
		return result;
	}

	void ImageMetadata::Deserialise(std::string data) {
		AssetMetadata::Deserialise(data);
	}

	Common::Ref<Image> Image::FromPath(std::filesystem::path texturePath) {
		if (LoadedImages.contains(texturePath.string())) return LoadedImages.at(texturePath.string());

		auto tex = new Image();
		tex->LoadAsset(texturePath);
		LoadedImages[texturePath.string()] = tex;

		return tex;
	}

	Image::~Image() {
		auto sourcePath = mMetadata.SourcePath.string();
		if (LoadedImages.contains(sourcePath)) {
			LoadedImages.erase(sourcePath);
		}
	}

	void Image::LoadAsset(const std::filesystem::path& source) {
		Asset::LoadAsset(source);

		mTexture = Engine::Platform::ATexture::FromPath(source);
		auto dim = mTexture.Get()->GetSize();
		mMetadata.Width = (int)dim.x;
		mMetadata.Height = (int)dim.y;
	}
}
