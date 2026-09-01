#pragma once

#include <map>

#include <Core/Common.h>
#include <Core/FileHandling.h>
#include <Classes/Assets/Asset.h>

namespace Refraction::Engine {
	// Instanced project singleton handling the AssetMap and Assets directory

	// TODO: create Metadata map, assets should refer to this and hopefully improves polymorphism with metadata objects
	class AssetManager {
	public:
		static Common::Ref<AssetManager> GetInstance();

		AssetManager(std::filesystem::path projectPath) : mProjectPath(projectPath) {}
		~AssetManager() = default;

		// Get asset by UUID
		template <typename AssetType> requires Common::DerivesFrom<Assets::Asset, AssetType>
		Common::Ref<AssetType> GetAsset(UUIDValue uuid) {
			auto& ref = mAssetMap.at(uuid);
			if (ref) return Common::NewRef<AssetType>(ref);
			// Asset isn't in memory, try finding and loading it
			if (auto metaPath = FindMetadataFile(uuid); std::filesystem::exists(metaPath)) {
				Log::SInfo("Asset with UUID " + std::to_string(uuid) + " is unloaded, registering now");
				return Common::NewRef<AssetType>(RegisterAsset(metaPath));
			}
			return nullptr;
		}

		// Get asset by path
		template <typename AssetType> requires Common::DerivesFrom<Assets::Asset, AssetType>
		Common::Ref<AssetType> GetAsset(std::filesystem::path assetPath) {
			// If invalid path, try appending to assets directory path
			if (!IsValidAsset(assetPath)) assetPath = mProjectPath / "Assets" / assetPath;
			for (auto& assetPair : mAssetMap) {
				auto& asset = assetPair.second;
				auto meta = FetchMetadata(asset->GetUUID());
				if (meta->AssetPath == assetPath) {
					Log::SInfo("Asset at path " + assetPath.string() + " is unloaded, registering now");
					return Common::NewRef<AssetType>(asset);
				}
			}
			// Asset isn't in memory, try loading it
			if (IsValidAsset(assetPath)) {
				auto metaPath = GetMetadataPath(assetPath);
				if (!metaPath) return nullptr;
				Log::SInfo("Asset at path " + assetPath.string() + " is unloaded, registering now");
				return Common::NewRef<AssetType>(RegisterAsset(metaPath.value()));
			}
			return nullptr;
		}

		// Attempts to fetch an asset's metadata with the given UUID
		template <typename MetadataType> requires Common::DerivesFrom<Assets::AssetMetadata, MetadataType>
		Common::Ref<MetadataType> FetchMetadata(UUIDValue uuid) {
			// Try searching map
			if (mMetadataMap.contains(uuid)) return Common::NewRef<MetadataType>(mMetadataMap.at(uuid));
			// Not loaded, try deep search on disk
			auto meta = RecursiveFindMetadataByUUID(mProjectPath / "Assets", uuid);
			if (meta) {
				// Save in memory
				mMetadataMap[uuid] = meta;
				return Common::NewRef<MetadataType>(meta);
			}
			// Not found
			return nullptr;
		}
		Common::Ref<Assets::AssetMetadata> FetchMetadata(UUIDValue uuid) { return FetchMetadata<Assets::AssetMetadata>(uuid); }

		// Loads all assets into memory using their metadata under the project folder
		void RegisterAllAssets();
		// Loads the specified asset into memory using its metadata
		Common::Ref<Assets::Asset> RegisterAsset(std::filesystem::path metadataPath);

		// Determines whether the asset is valid (has metadata file, in current project, etc)
		bool IsValidAsset(std::filesystem::path assetPath);
		// Returns path of the asset's metadata file
		std::optional<std::filesystem::path> GetMetadataPath(std::filesystem::path assetPath);
	private:
		std::map<UUIDValue, Common::Shared<Assets::Asset>> mAssetMap = {};
		std::map<UUIDValue, Common::Shared<Assets::AssetMetadata>> mMetadataMap = {};
		std::filesystem::path mProjectPath;

		void RecursiveRegisterAssets(std::filesystem::path folder);
		Common::Shared<Assets::AssetMetadata> RecursiveFindMetadataByUUID(std::filesystem::path folder, UUIDValue uuid);

		Common::Shared<Assets::AssetMetadata> LoadMetadata(std::filesystem::path metadataPath);

		// Attempts to find an asset's metadata file with the given UUID
		inline std::filesystem::path FindMetadataFile(UUIDValue uuid) {
			auto metaRef = FetchMetadata(uuid);
			if (auto meta = metaRef.lock()) return meta->GetPath();
			return "";
		}
	};
}
