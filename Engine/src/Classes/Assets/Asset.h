#pragma once

#include <string>
#include <unordered_map>

#include <Core/Common.h>
#include <Core/UUID.h>
#include <Core/FileHandling.h>

#define REFRACTION_ASSET_METADATA_EXTENSION ".rfmeta"

namespace Refraction::Assets {
	struct AssetMetadata {
		UUID AssetUUID;
		std::filesystem::path SourcePath = ""; // Path to the original source file
		std::filesystem::path AssetPath = ""; // Path to the actual asset file in the project
		uintmax_t FileSize = 0;

		AssetMetadata() = default;
		~AssetMetadata() = default;

		// Serialises metadata into a string
		virtual std::string Serialise();
		// Deserialises AND LOADS metadata from the given string
		virtual void Deserialise(std::string data);
	};

	class Asset {
	public:
		Asset() = default;
		~Asset() = default;

		template <typename AssetType>
		static Common::Ref<AssetType> GetAsset(uint64_t uuid) {
			auto& asset = AssetMap.at(uuid);
			if (!asset) return nullptr;
			return dynamic_pointer_cast<AssetType>(asset);
		}

		virtual AssetMetadata& GetMetadata() { return mMetadata; }

		// Loads asset from the provided source file
		virtual void LoadAsset(const std::filesystem::path& source);
		// Loads asset from the provided source file
		void LoadAsset(std::string source) { return LoadAsset(std::filesystem::path(source)); }
		// Saves the asset to disk
		virtual void Save();
	protected:
		std::string mDisplayName;

		static inline void AddToMap(Asset* asset) {
			AssetMap.insert(std::make_pair(asset->GetMetadata().AssetUUID.AsInt(), Common::Ref<Asset>(asset)));
		}
	private:
		static std::unordered_map<uint64_t, Common::Ref<Asset>> AssetMap;

		AssetMetadata mMetadata;
	};
}