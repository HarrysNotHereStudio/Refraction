#pragma once

#include <string>

#include <Classes/Assets/Image.h>
#include <Platform/AMeshFragment.h>

#include "Asset.h"

namespace Refraction::Assets {
	struct ModelMetadata : public AssetMetadata {
		int VertexCount = 0;
		int PolyCount = 0;

		ModelMetadata() = default;
		~ModelMetadata() = default;

		nlohmann::json Serialise() override;
		void Deserialise(std::string data) override;
	};

	class Model : public Asset {
	public:
		std::vector<Common::Shared<Engine::Platform::AMeshFragment>> mFragments;

		Model() = default;
		~Model() override = default;

		MetadataType GetMetadataType() override { return MetadataType::Model; }
	protected:
		std::vector<Common::Ref<Assets::Material>> mMaterials;

		void OnLoadAsset(Common::Shared<AssetMetadata> metadata) override;
	private:
		unsigned int mID = 0;
	};
}
