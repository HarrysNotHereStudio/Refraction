#include "AssetManager.h"

namespace Refraction::Engine {
	void AssetManager::RegisterAsset(Common::Ref<Assets::Asset> asset) {
		auto& meta = asset->GetMetadata();
		auto& uuid = meta.AssetUUID;
		if(uuid.IsValid()) mAssetMap.insert(std::make_pair(uuid.AsInt(), asset));
	}
}
