#include <unordered_map>

#include <Classes/ClassSerialiser.h>
#include <Interface/AssetManager.h>

#include "Image.h"

namespace Refraction::Assets {
	std::string ImageMetadata::Serialise() {
		return Utilities::ClassSerialiser::TryAppendJSON(AssetMetadata::Serialise(), [&](nlohmann::json& json) {
			json["Width"] = Width;
			json["Height"] = Height;
			json["Channels"] = Channels;
		});
	}

	void ImageMetadata::Deserialise(std::string data) {
		AssetMetadata::Deserialise(data);
		Utilities::ClassSerialiser::TryParseJSON(data, [&](nlohmann::json& json) {
			Width = json.at("Width").get<int>();
			Height = json.at("Height").get<int>();
			Channels = json.at("Channels").get<int>();
		});
	}

	Image::~Image() {}

	void Image::OnLoadAsset(Common::Shared<AssetMetadata> metadata) {
		auto meta = Common::AsA<ImageMetadata>(metadata);
		if (!meta) {
			Log::SError("Metadata cast failed");
			return;
		}

		mTexture = Engine::Platform::ATexture::FromPath(meta->AssetPath);
		if (mTexture.expired()) throw Common::RuntimeError("Failed to generate texture from path");
		auto dim = mTexture.lock()->GetSize();
		meta->Width = (int)dim.x;
		meta->Height = (int)dim.y;
	}
}
