#include <json.hpp>

#include <Core/FileHandling.h>
#include <Classes/ClassSerialiser.h>
#include <Classes/ClassHeaders.h>
#include <Interface/AssetManager.h>

#include "Asset.h"

namespace Refraction::Assets {
	Common::Shared<AssetMetadata> AssetMetadata::CastedDeserialise(std::string data) {
		std::string metaType;
		Utilities::ClassSerialiser::TryParseJSON(data, [&](nlohmann::json& json) {
			metaType = json.at("MetadataType").get<std::string>();
		});

		Common::Shared<AssetMetadata> result;
		if (metaType == typeid(AssetMetadata).name()) {
			result = Common::NewShared<AssetMetadata>();
		} else if (metaType == typeid(ImageMetadata).name()) {
			result = Common::NewShared<ImageMetadata>();
		} else if (metaType == typeid(ShaderMetadata).name()) {
			result = Common::NewShared<ShaderMetadata>();
		} else if (metaType == typeid(ModelMetadata).name()) {
			result = Common::NewShared<ModelMetadata>();
		}

		result->Deserialise(data);
		return result;
	}

	std::filesystem::path AssetMetadata::GetPath() const {
		if (AssetPath.empty()) return "";
		return AssetPath.stem().string() + RFCT_ASSET_METADATA_EXTENSION;
	}

	std::string AssetMetadata::Serialise() {
		return Utilities::ClassSerialiser::TryAppendJSON("{}", [&](nlohmann::json& json) {
			json["MetadataType"] = typeid(*this).name();
			json["AssetType"] = AssetType;
			json["AssetUUID"] = AssetUUID.Serialise();
			json["SourcePath"] = SourcePath.string();
			json["AssetPath"] = AssetPath.string();
			json["FileSize"] = FileSize;
		});
	}

	void AssetMetadata::Deserialise(std::string data) {
		Utilities::ClassSerialiser::TryParseJSON(data, [&](nlohmann::json& json) {
			AssetType = json["AssetType"].get<std::string>();
			AssetUUID = UUID::Deserialise(json["AssetUUID"]);
			SourcePath = std::filesystem::path(json["SourcePath"].get<std::string>());
			AssetPath = std::filesystem::path(json["AssetPath"].get<std::string>());
			FileSize = json["FileSize"].get<uintmax_t>();
		});
	}

	Asset::~Asset() {}

	void Asset::LoadAsset(UUIDValue uuid) {
		Common::Ref<AssetMetadata> metaWeak;
		Engine::AssetManager::Try([&](Common::Shared<Engine::AssetManager> assetManager) {
			metaWeak = assetManager->FetchMetadata(uuid);
		});

		if (auto meta = metaWeak.lock()) {
			mUUID = uuid;
			if (!std::filesystem::exists(meta->SourcePath) && !std::filesystem::exists(meta->AssetPath)) throw std::runtime_error("Failed to load asset with provided metadata, no asset path exists.");

			// Derived object actually loads asset
			OnLoadAsset(meta);
		} else {
			throw Common::RuntimeError("Failed to load asset with UUID " + UUID::AsString(uuid) + ", could not fetch metadata");
		}
	}

	void Asset::Save() {
		OnSave();
	}

	void Asset::MakeVolatile() {
		mVolatile = true;
		mUUID = UUID::UUID();

		OnMakeVolatile();
	}
}
