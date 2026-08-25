#include <json.hpp>

#include <Core/FileHandling.h>

#include "Asset.h"

using nlohmann::json;

namespace Refraction::Assets {
	std::string AssetMetadata::Serialise() {
		json serialised;

		serialised["AssetUUID"] = AssetUUID.AsInt();
		serialised["SourcePath"] = SourcePath.string();
		serialised["AssetPath"] = AssetPath.string();
		serialised["FileSize"] = FileSize;

		return serialised.dump();
	}
	void AssetMetadata::Deserialise(std::string data) {
		try {
			json serialised = json::parse(data);
			AssetUUID = UUID::Deserialise(serialised["AssetUUID"]);
			SourcePath = std::filesystem::path(serialised["SourcePath"].get<std::string>());
			AssetPath = std::filesystem::path(serialised["AssetPath"].get<std::string>());
			FileSize = serialised["FileSize"].get<uintmax_t>();
		} catch (const json::parse_error& err) {
			Log::SError("Failed to parse asset metadata: " + std::string(err.what()));
		}

		return;
	}

	Asset::~Asset() {
		auto uuid = mMetadata.AssetUUID.AsInt();
	}

	void Asset::LoadAsset(const std::filesystem::path& source) {
		if (!std::filesystem::exists(source)) throw std::runtime_error("Invalid asset path " + source.string());
		// Reset loaded metadata
		auto& metadata = GetMetadata();
		metadata = AssetMetadata();

		if (source.extension() == REFRACTION_ASSET_METADATA_EXTENSION) { // Load a metadata file
			metadata.Deserialise(FileHandling::ReadFile(source));
		} else { // Creating a new asset
			metadata.SourcePath = source;
			metadata.AssetPath = source;
			metadata.FileSize = std::filesystem::file_size(source);
		}
		auto newUUID = GetMetadata().AssetUUID.AsInt();
	}

	void Asset::Save() {

	}
}
