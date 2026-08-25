#pragma once

#include <map>

#include <Core/Common.h>
#include <Classes/Assets/Asset.h>

namespace Refraction::Engine {
	// Instanced project singleton handling the AssetMap
	class AssetManager {
	public:
		AssetManager() = default;
		~AssetManager() = default;

		template <typename AssetType>
		Common::Ref<AssetType> GetAsset(uint64_t uuid) {
			return dynamic_pointer_cast<AssetType>(mAssetMap.at(uuid));
		}

		void RegisterAsset(Common::Ref<Assets::Asset> asset);
	private:
		std::map<uint64_t, Common::Ref<Assets::Asset>> mAssetMap = {};
	};
}
