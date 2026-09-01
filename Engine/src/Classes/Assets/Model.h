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

		std::string Serialise() override;
		void Deserialise(std::string data) override;
	};

	class Model : public Asset {
	public:
		Model() = default;
		virtual ~Model() = default;

	protected:
		std::vector<Common::Shared<Engine::Platform::AMeshFragment>> mFragments;
		std::vector<Common::Ref<Assets::Image>> mTextures;

		void InternalLoadAsset(Common::Ref<AssetMetadata> metadata) override;
	private:
		unsigned int mID = 0;
	};
}
