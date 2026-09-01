#include <Classes/ClassSerialiser.h>
#include <Interface/Project.h>

#include "AssetManager.h"

namespace Refraction::Engine {
	Common::Shared<AssetManager> AssetManager::GetInstance() {
		if (auto project = Project::GetCurrent()) {
			return project->GetAssetManager();
		}
		return nullptr;
	}

	void AssetManager::RegisterAllAssets() {
		Log::SInfo("Registering all assets in project...");
		RecursiveRegisterAssets(mProjectPath / "Assets");
	}

	Common::Shared<Assets::Asset> AssetManager::RegisterAsset(std::filesystem::path metadataPath) {
		auto pathStr = metadataPath.string();
		if (!(std::filesystem::exists(metadataPath) && std::filesystem::is_regular_file(metadataPath) && metadataPath.has_extension() && metadataPath.extension() == RFCT_ASSET_METADATA_EXTENSION)) {
			Log::SError("Invalid metadata file at path " + pathStr);
			return nullptr;
		}

		auto meta = LoadMetadata(metadataPath);
		auto asset = Utilities::ClassSerialiser::DeserialiseAsset(meta);
		auto& uuid = meta->AssetUUID;
		if (!mAssetMap.count(uuid)) {
			mAssetMap[uuid] = asset;
			return asset;
		} else {
			Log::SWarn("Asset with UUID " + uuid.AsString() + " already exists, returning existing asset");
			return mAssetMap.at(uuid);
		}
	}

	bool AssetManager::IsValidAsset(std::filesystem::path assetPath) {
		auto metaPath = GetMetadataPath(assetPath);
		if (!metaPath) return false;
		// too lazy to figure out if assetPath is within mProjectPath
		// just pray for the best bro
		return true;
	}

	std::optional<std::filesystem::path> AssetManager::GetMetadataPath(std::filesystem::path assetPath) {
		auto assetName = assetPath.filename().string();
		// Make sure it's a full path before searching it
		if (!std::filesystem::exists(assetPath)) assetPath = mProjectPath / "Assets" / assetPath;
		auto files = FileHandling::GetFilesOfExtInFolder(assetPath.parent_path(), RFCT_ASSET_METADATA_EXTENSION);
		for (auto& file : files) {
			if (file.path().filename().string() == assetName) return std::make_optional(file.path());
		}
		return std::nullopt;
	}

	void AssetManager::RecursiveRegisterAssets(std::filesystem::path folder) {
		auto metaFiles = FileHandling::GetFilesOfExtInFolder(folder, RFCT_ASSET_METADATA_EXTENSION);
		for (auto& metaFile : metaFiles) {
			RegisterAsset(metaFile);
		}
		auto folders = FileHandling::GetFoldersInFolder(folder);
		for (auto& child : folders) {
			RecursiveRegisterAssets(child);
		}
	}

	Common::Shared<Assets::AssetMetadata> AssetManager::RecursiveFindMetadataByUUID(std::filesystem::path folder, UUIDValue uuid) {
		auto metaFiles = FileHandling::GetFilesOfExtInFolder(folder, RFCT_ASSET_METADATA_EXTENSION);
		for (auto& metaFile : metaFiles) {
			auto dataStr = FileHandling::ReadFile(metaFile);
			auto meta = Assets::AssetMetadata::CastedDeserialise(dataStr);
			// Return result
			// TODO: Optimise by only doing CastedDeserialise after testing UUID
			if (meta->AssetUUID.AsInt() == uuid) return meta;
		}
		// Continue searching
		auto folders = FileHandling::GetFoldersInFolder(folder);
		for (auto& child : folders) {
			RecursiveFindMetadataByUUID(child, uuid);
		}
		return nullptr;
	}

	Common::Shared<Assets::AssetMetadata> AssetManager::LoadMetadata(std::filesystem::path metadataPath) {
		if (!std::filesystem::exists(metadataPath)) {
			Log::SError("Could not find metadata file at path " + metadataPath.string());
			return nullptr;
		}
		auto dataStr = FileHandling::ReadFile(metadataPath);

		// TODO: Optimise by checking if UUID already exists in map before doing CastedDeserialise
		auto meta = Assets::AssetMetadata::CastedDeserialise(dataStr);

		// Save in memory
		mMetadataMap[meta->AssetUUID] = meta;
		return meta;
	}
}
