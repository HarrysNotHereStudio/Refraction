#pragma once

#include <string>
#include <map>

#include <Core/Common.h>
#include <Core/UUID.h>
#include <Core/FileHandling.h>

#define RFCT_ASSET_METADATA_EXTENSION ".rfmeta"

namespace Refraction::Assets {
	struct AssetMetadata {
		UUID AssetUUID = UUID::Null();
		std::filesystem::path SourcePath = ""; // Path to the original source file
		std::filesystem::path AssetPath = ""; // Path to the actual asset file in the project
		std::string AssetType = "";
		uintmax_t FileSize = 0;

		// Returns a deserialised (derived) AssetMetadata object
		static Common::Shared<AssetMetadata> CastedDeserialise(std::string data);

		AssetMetadata() = default;
		~AssetMetadata() = default;

		// Returns path to the metadata file
		std::filesystem::path GetPath() const;

		// Serialises metadata into a string
		virtual std::string Serialise();
		// Loads metadata from the given string
		virtual void Deserialise(std::string data);
	};

	class Asset {
	public:
		Asset() = default;
		virtual ~Asset();

		// Loads asset of the provided UUID from disk
		void LoadAsset(UUIDValue uuid);
		// Saves asset changes to disk
		virtual void Save();

		inline bool IsVolatile() const { return mVolatile; }
		inline UUIDValue GetUUID() const { return mUUID; }
	protected:
		std::string mDisplayName;
		// Asset does not have a location on disk if true
		bool mVolatile = false;

		// Actual loading functionality of derived objects
		virtual void InternalLoadAsset(Common::Shared<AssetMetadata> metadata) {}
	private:
		UUIDValue mUUID = 0;
	};
}